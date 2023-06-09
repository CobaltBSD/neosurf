# NetSurf bitmap decoding library

The functions provided by this library allow for the decoding of
Microsoft's BMP and ICO image file formats.

## Reading bmp files
To begin decoding a BMP, the caller should initialise a `bmp_bitmap_callback_vt` structure with the appropriate values necessary to handle bitmap images.  Next, a `bmp_image` structure should be initialised by calling `bmp_create()`.  This structure should then be passed to `bmp_analyse()` along with the BMP data to process and the size of this data.

Once the analysis has begun, the decoder completes the width and height
variables.

To decode the image, the caller must use `bmp_decode()` which selects the proper decoding method based on the BMP info header and assigns the decoded bitmap image to the `bitmap` member of the `bmp_image` structure.  The bitmap image is stored with 4 bytes-per-pixel in RGBA format.

It should be noted that `bmp_finalise()` should always be called, even if the image was never decoded.  It is also the responsibility of the caller to free `bmp_data`.

## Reading ico files
To begin decoding an ICO, the caller should initialise a `bmp_bitmap_callback_vt` structure with the appropriate values necessary to handle bitmap images.  Next, an `ico_collection` structure should be initialised by calling `ico_create()`.  This structure should then be passed to `ico_analyse()` along with the ICO data to process and the size of this data.

Once the analysis has begun, the decoder completes the width and height variables.  Because ICO collections contain multiple bitmap images, the width and height will contain the values of the largest available image.

The caller then obtains a BMP from the ICO collection by calling `ico_find()` with the requested width and height.

To decode the image, the caller must use `bmp_decode()` which selects the proper decoding method based on the BMP info header and assigns the decoded bitmap image to the `bitmap` member of the `bmp_image` structure.  The bitmap image is stored with 4 bytes-per-pixel in RGBA format.

It should be noted that `ico_finalise()` should always be called, even if no images were decoded.  Because `ico_finalise()` calls `bmp_finalise()` for each bitmap within the collection, the caller is not required to perform this function.  However, it is the responsibility of the caller to free `ico_data`.
