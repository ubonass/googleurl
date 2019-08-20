# googleurl

This is a copy of [Chrome's URL parsing
library](https://cs.chromium.org/chromium/src/url/), adapted to work with
[Bazel](https://bazel.build/).  It is meant to be used by
[QUICHE](https://quiche.googlesource.com/quiche/+/refs/heads/master), but can be
also used by other projects that use Bazel.

In order to be used successfully, `-std=c++14` and `-fno-strict-aliasing`
compile flags are required.

For questions, contact <proto-quic@chromium.org>.
