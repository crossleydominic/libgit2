#ifndef INCLUDE_codeowners_h
#define INCLUDE_codeowners_h

extern int git_codeowners_foreach_owner(
  const char *path,
  const char *codeowners,
  int (*callback)(const char *path_name, const char *owner_name, void *payload),
  void *payload
);

#endif
