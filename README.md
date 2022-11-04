# PPM-Image-Editor

Implemented a program in C that can compress,decompress and mirror, both horizontally and vertically, images in PPM format, using Quadtree Data Strcutures.

The compreesion proccess stops when arithmetic mean of the values from pixel matrix corresponding to a block of the image is less than the threshold given by user.

After the compression, the output is written in a binary file, as an array of Quadtree elements, which size is equal to the number of nodes of the resulted Quadtree.

The decompression algorithm restores an image from a compressed file.

The mirroring function compresses the image , changing the proccessing order of the pixel blocks so that the resulted array of Quadtree cointains the pixels in the requested order.
