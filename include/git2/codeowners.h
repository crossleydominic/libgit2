#ifndef INCLUDE_git_codeowners_h__
#define INCLUDE_git_codeowners_h__

#include "common.h"
#include "types.h"

GIT_BEGIN_DECL

/**
 * The callback used with git_codeowners_foreach_owner.
 *
 * This callback will be invoked only once per owner name, even if there
 * are multiple rules for a given file. The highest priority rule will be
 * used.
 *
 * @see git_codeowners_foreach_owner_cb.
 *
 * @param path_name The path name.
 * @param owner_name The owner name
 * @param payload A user-specified pointer.
 * @return 0 to continue looping, non-zero to stop. This value will be returned
 *         from git_codeowners_foreach_owner.
 */
typedef int GIT_CALLBACK(git_codeowners_foreach_owner_cb)(const char *path_name, const char *owner_name, void *payload);

/**
 * Loop over all the code owners for a path.
 *
 * @param path The path to retrieve owner information for
 * @param codeowners The path to a codeowners file within a repository.
 * @param callback Function to invoke on each owner name.
 *                 See git_codeowners_foreach_owner_cb.
 * @param payload Passed on as extra parameter to callback function.
 * @return 0 on success, non-zero callback return value, or error code
 */
GIT_EXTERN(int) git_codeowners_foreach_owner(
  const char *path,
  const char *codeowners,
  git_codeowners_foreach_owner_cb callback,
  void *payload
);

GIT_END_DECL
#endif


