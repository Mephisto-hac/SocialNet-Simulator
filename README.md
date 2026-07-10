# SocialNet Simulator — 

A command-line social network backend built using a self-implemented **Graph**
(adjacency list, for friendships) and a self-implemented **AVL Tree** (for
storing each user's posts, ordered by creation time).

## Files

```
SocialNetSimulator/
├── main.cpp             # entry point, calls SocialNetwork::run()
├── AVLTree.hpp           # AVLNode struct + AVLTree class (posts, self-balancing)
├── Graph.hpp             # Graph class (adjacency list of unordered_set<int>)
├── User.hpp               # User struct: username + AVLTree of posts
├── SocialNetwork.hpp    # command parsing + all command logic
├── compile.sh              # build script
└── README.md
```

Only `std::unordered_map` / `std::unordered_set` are used from the STL, for
hashing usernames to vertex ids and storing adjacency lists — the Graph and
AVL Tree themselves are implemented from scratch, per the assignment spec.

## How to Compile

```bash
bash compile.sh
```

This runs:
```bash
g++ -std=c++17 -O2 -o socialnet main.cpp
```

`-std=c++17` is required (structured bindings are used in the BFS for
`DEGREES OF SEPARATION`).


## How to Run

The program reads commands from standard input, one per line, until EOF.

**Interactive mode** (type commands manually; press Ctrl+D on Linux/Mac,
or Ctrl+Z then Enter on Windows, to signal end of input):
```bash
./socialnet
```


## Commands

Usernames are **not case-sensitive** — `Alice` and `ALICE` refer to the
same user.

### Social Network Operations

`ADD USER <username>`
Adds a new user with no friends and no posts.

`ADD FRIEND <username1> <username2>`
Creates a bidirectional friendship between two existing users.

`LIST FRIENDS <username>`
Prints the user's friends, alphabetically sorted.

`SUGGEST FRIENDS <username> <N>`
Suggests up to N "friend of a friend" users, ranked by number of mutual
friends (descending), ties broken alphabetically by username. Prints
nothing if N is 0.

`DEGREES OF SEPARATION <username1> <username2>`
Prints the shortest friendship path length between two users (unweighted
BFS), or `-1` if no path exists.

### User Content Operations

`ADD POST <username> "<post content>"`
Adds a post to the user's posts (post content must be in double quotes).

`OUTPUT POSTS <username> <N>`
Prints the user's N most recent posts, newest first. Use `N = -1` to print
all posts. If fewer than N posts exist, prints all available.

## Example

```
ADD USER Alice
ADD USER Bob
ADD USER Charlie
ADD FRIEND alice bob
ADD FRIEND alice charlie
LIST FRIENDS alice
ADD POST alice "hello world"
ADD POST alice "second post"
OUTPUT POSTS alice -1
DEGREES OF SEPARATION bob charlie
```

Expected output:
```
bob charlie
second post
hello world
2
```

## Design Notes

- **Friendship data lives only in `Graph`.** `User` does not keep its own
  friend list — it only stores a username and an `AVLTree` of posts. This
  avoids two sources of truth (Graph and User) drifting out of sync as
  friendships change.
- **Post ordering uses `time_t` timestamps, with ties broken toward
  insertion order.** Since several `ADD POST` commands can run within the
  same clock second, timestamps can be equal. The AVL insert comparison
  uses `<=` (not `<`) when deciding whether a new post goes to the right
  subtree, so on a tie the new post always lands to the right of the
  existing one with that timestamp. Since the system clock is
  monotonically non-decreasing during a single run, this makes ties
  consistently resolve in insertion order, so a reverse in-order
  traversal always yields newest-first output — regardless of how many
  AVL rotations have occurred.
- **Vertex ids are assigned in the same order users are added**, and
  `usernameWithId` (an `unordered_map<string,int>`) maps a lowercased
  username to its vertex id / index into the `users` vector.

## Edge Cases Handled

- Invalid usernames (never added) are rejected with an error instead of
  crashing or silently creating bad state, across every command.
- `DEGREES OF SEPARATION` correctly returns `0` for the same user and
  `-1` when no path exists between two users.
- `SUGGEST FRIENDS` and `OUTPUT POSTS` degrade gracefully when there are
  fewer results than requested (e.g. N = 0, no friends, no posts, or
  N larger than what's available).
