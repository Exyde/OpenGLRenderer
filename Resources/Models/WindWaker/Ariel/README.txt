Pirate dress included. Just change the body texture from lsbody01.png to lsbody02.png.

Eyebrow image texture should be set to extend/clamp instead of repeat/tile.

Don't forget about the alpha settings for the eye material.

The pupil texture (ls_hitomi.1.png) goes on the eye mesh somehow. I don't know how it's "supposed" to be done, but my method, in Blender 3.5.0, is to add the pupil image texture to the eye shader and connect both textures to a Color Mixer node set to Darken and the factor set to 1.0. You can change the position of the pupils by plugging a UV Map input node into the Vector input of a Vector Mapping node, and plugging that into the Vector input of the pupil texture.