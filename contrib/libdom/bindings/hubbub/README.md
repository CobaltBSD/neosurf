# Hubbub binding for libdom

This is a wrapper around hubbub's parser API, to facilitate construction of a libdom DOM tree. The basic premise is that the wrapper intercepts the SAX-like events emitted by hubbub's tokeniser then builds a libdom DOM tree from them.

