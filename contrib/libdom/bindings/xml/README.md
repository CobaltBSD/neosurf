# LibXML binding for libdom

This is a wrapper around libxml's push parser API, to facilitate construction of a libdom DOM tree. The basic premise is that the wrapper intercepts the SAX events emitted by libxml's tokeniser then invokes  libxml's own SAX handlers, wrapping the results up in libdom-specific data structures.

The tree created is thus a tree of libdom nodes, each of which is linked to the libxml node that backs it. This allows the binding to process the DOM tree using libxml api, should it need to (e.g. for normalization purposes).

