#include "clar_libgit2.h"
#include "codeowners.h"

#define MAX_CODEOWNERS_EXPECTED 2

struct codeowners_expected {
  const char *path;
  const char *expected[MAX_CODEOWNERS_EXPECTED];
};

struct codeowner_actual {
  int count;
  const char *owner[256];
};

void reset_actual(struct codeowner_actual *a) {
  a->count = 0;
  memset(&a->owner, 0, 256*sizeof(*a->owner));
}

static int record_owner(
  const char *path,
  const char *owner,
  void *payload) {

  struct codeowner_actual *actual =
    (struct codeowner_actual *)payload;

  actual->owner[actual->count] = owner;
  actual->count++;

  return 0;
}

void test_codeowners_read__testLoad2(void) {
  struct codeowners_expected cases[] = {
    // * rule
    { "a", {"@global-owner1", "@global-owner2"} },
    { "a/b", {"@global-owner1", "@global-owner2"} },
    { "a/b/c/", {"@global-owner1", "@global-owner2"} },
    { "a/b/c/d.hs", {"@global-owner1", "@global-owner2"} },

    // *.js rule
    { "a.js", {"@js-owner", NULL, } },
    { "a/b.js", {"@js-owner", NULL, } },
    { "a/b/c.js", {"@js-owner", NULL, } },
    { "a/b/c/d.js", {"@js-owner", NULL, } },
    { "a/b/c/d.js/", {"@js-owner", NULL, } },
    { "a/b/c/d.js/e", {"@js-owner", NULL, } },
    { "a/b/c/d.js/e.js", {"@js-owner", NULL, } },

    // *.go rule
    { "a.go", {"docs@example.com", NULL, } },
    { "a/b.go", {"docs@example.com", NULL, } },
    { "a/b/c.go", {"docs@example.com", NULL, } },
    { "a/b/c/d.go", {"docs@example.com", NULL, } },
    { "a/b/c/d.go/", {"docs@example.com", NULL, } },
    { "a/b/c/d.go/e", {"docs@example.com", NULL, } },
    { "a/b/c/d.go/e.go", {"docs@example.com", NULL, } },

    // /build/logs/ rule
    { "build/logs", {"@global-owner1", "@global-owner2"} }, //File, not dir!
    { "build/logs/", {"@doctocat", NULL} },
    { "build/logs/a", {"@doctocat", NULL} },
    { "build/logs/a/b", {"@doctocat", NULL} },
    { "build/logs/a/b/", {"@doctocat", NULL} },
    { "build/logs/a/b/c.js", {"@doctocat", NULL} },
    { "build/logs/a/b/c.go", {"@doctocat", NULL} },
    { "build/logs/a/b/c.hs", {"@doctocat", NULL} },
    { "a/build/logs/b", {"@global-owner1", "@global-owner2"} },
    { "a/build/logs/b/c", {"@global-owner1", "@global-owner2"} },
    { "a/build/logs/", {"@global-owner1", "@global-owner2"} },
    { "a/build/logs", {"@global-owner1", "@global-owner2"} },

    // docs/* rule
    { "docs", {"@global-owner1", "@global-owner2"} }, //File, not dir!
    { "docs/", {"@docs-finally", NULL} }, //See the bottom rule!
    { "docs/a", {"@docs-finally", NULL} }, //See the bottom rule!

    { "a/docs", {"@global-owner1", "@global-owner2"} }, //File, not dir!
    { "a/docs/", {"@global-owner1", "@global-owner2"} }, //Rule matches everything under it but not itself
    { "a/docs/c/d", {"@global-owner1", "@global-owner2"} }, //Rule does not match nested dirs.
    //{ "a/docs/b", {"docs-again@example.com", NULL} }, //BROKEN!

    { "apps/a", {"@octocat", NULL} },
    { "apps/a/b", {"@octocat", NULL} },
    { "a/apps/b", {"@octocat", NULL} },
    { "a/apps/b/c.js", {"@octocat", NULL} }, //This rule takes precedence over the above *.js rule

    { "docs", {"@global-owner1", "@global-owner2"} }, //File, not dir!
    { "docs/", {"@docs-finally", NULL} },
    { "docs/a", {"@docs-finally", NULL} },
    { "a/docs/b", {"@global-owner1", "@global-owner2"} },


    //Tombstone entry
    { NULL, { NULL } }
  };

  struct codeowners_expected *c;
  struct codeowner_actual actual;
  int i;

  for (c = cases; c->path != NULL; c++) {
    reset_actual(&actual);

    git_codeowners_foreach_owner(
      c->path,
      cl_fixture("codeowners/CODEOWNERS"),
      &record_owner,
      &actual);

    for(i = 0; i < MAX_CODEOWNERS_EXPECTED; i++){
      cl_assert_equal_s(actual.owner[i], c->expected[i]);
    }
  }
}


