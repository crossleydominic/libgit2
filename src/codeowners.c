#include "attr_file.h"
#include "codeowners.h"
#include "git2/codeowners.h"

bool move_path_up_directory(git_attr_path *path){
  if (path->basename == path->path) {
    return false; //Can't move up anymore
  }

  path->basename[-1] = '\0';
  while (path->basename > path->path && *path->basename != '/')
    path->basename--;
  if (path->basename > path->path)
    path->basename++;
  path->is_dir = 1;

  return true; //Move succeeded
}

bool is_dir(const char* path_name) {
	return (!git__suffixcmp(path_name, "/")) ?
		GIT_DIR_FLAG_TRUE :
		GIT_DIR_FLAG_FALSE;
}

int git_codeowners_foreach_owner(
  const char* path_name,
  const char* codeowners_path,
  int (*callback)(const char *path_name, const char *owner_name, void *payload),
  void *payload) {

	git_attr_file *codeowners_file;
	int error;
	git_attr_path path;
  size_t i, j;
  git_attr_rule *rule;
  git_attr_assignment *assignment;
	git_dir_flag dir_flag;

  assert (path_name && codeowners_path && callback);

  dir_flag = is_dir(path_name);

	if ((error = git_attr_file__load_standalone(&codeowners_file, codeowners_path)) < 0) {
    return error;
  }

  git_vector_rforeach(&codeowners_file->rules, j, rule) {
    if ((error = git_attr_path__init(&path, path_name, NULL, dir_flag)) < 0) {
      goto out;
    }

    while(1) {
      if (!(rule->match.flags & GIT_ATTR_FNMATCH_DIRECTORY &&
          path.is_dir == GIT_DIR_FLAG_FALSE)) {

        if (git_attr_fnmatch__match(&rule->match, &path) &&
           ((rule->match.flags & GIT_ATTR_FNMATCH_NEGATIVE) == 0)) {

          git_vector_foreach(&rule->assigns, i, assignment) {

            if (assignment->value == git_attr__true) {
              if ((error = callback(path_name, assignment->name, payload)) <0) {
                goto out;
              }
            }
          }

          //Processed Ok, we're done.
          goto out;
        }
      }

      if (!move_path_up_directory(&path)) { break; }
    }
  }

out:
  if (error < 0) {
    git_attr_file__free(codeowners_file);
  }

  return error;
}

