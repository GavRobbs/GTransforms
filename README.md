# Transform.cpp

This was a practice project I used to help me understand linear transformations. Game engines like Godot and Unity use these in combination with their scene graphs to position objects. The GLM library was used so that I didn't have to rewrite my own 3d math suite, and it has some useful functions, such as glm::to_string that saved me a lot of time.

When you have a hierarchical structure, the transformation matrix of an object is generated from the combined transformation matrix of its parents plus its own local transformation matrix, and this is fed to the graphics engine as the model matrix (as in, the same model matrix that makes up model-view-projection).

$$
M_{local} = M_{scale} \times M_{rotation} \times M_{translation}
$$

$$
M_{model} = \prod_{i}^{n} M_{parent_i} * M_{local}
$$

The model matrix transforms vertex coordinates from object local space into world space.

$$
P_{world} = M_{model}\times P_{local}
$$

Naturally, the inverse model matrix can convert points from world space into object local space.

$$
P_{local} = M_{model}^{-1}\times P_{world}
$$

As objects rotate, their local axes change. If not represented properly, this can lead to Gimbal lock. Quaternions avoid that because they use a 4th dimension. This transform system also includes a hierarchy of rotations from parents to children. Be careful not to confuse rotation around an axis with orbiting around a point:
- When you rotate around an axis, the origin of the transform does not change position (eg. rotating a cube on the spot). 
- When you orbit around a point, the position does change (rotating the cube around the origin, this is more often than not
what you're be thinking about).

To clarify, if the parent has its own local rotation of 30 degrees around the y-axis and the child has its own local rotation of 45 degrees around the y-axis, the combined global rotation of the child is 75 degrees around the y-axis. This will not affect the position of the child, just the orientation/heading.

To build the library without running the tests, turn on the GTRANSFORMS_NO_TESTS flag. Tests are done using Google Test.

## Design Inspiration

The primary design inspiration for this came from how Unity structures its Transform class. Mine isn't as feature complete, but I think I've learnt what I need to from it. I may extend it in the future if/when I build my own scene graph (likely with OpenGL).

## Changes

- 8/1/24: Added the option to build it as a static library, without the example
- 8/9/24: Added some unit tests and integrate CI with GitHub actions

## Potential Improvements

- A dirty flag for optimization purposes, to prevent recalculation of the global transformation matrix or basis vectors every time one is requested
- Some unit tests (DONE)
- Convert it to a library instead of an executable (DONE)