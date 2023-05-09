# LibWapcaplet

LibWapcaplet provides a reference counted string internment system designed to store small strings and allow rapid comparison of them in terms of equality. It supports a caseless comparison where it will automatically intern a lowercased variant of the string and use that for comparison if needed.

## Rationale
Prior to LibWapcaplet, LibParserUtils contained a dictionary and hash implementation along with a red-black tree implementation internally. These three things were then used by client applications and libraries such as LibCSS. However, the code was deemed to be inefficient and the features in the wrong library. The behaviour required of the client libraries was therefore split out so that internment would still be able to be shared between different client libraries in the same application. (E.g. LibCSS and Hubbub)

For those interested, The name 'Wapcaplet' is from the Monty Python sketch in which Mr Simpson (who is not French) attempts to sell 122,000 miles of string which was unfortunately cut up into 3 inch lengths, and Adrian Wapcaplet comes up with the idea of "Simpson's individual emperor stringettes - Just the right length!"

## API documentation
For API documentation see `include/libwapcaplet/libwapcaplet.h`

