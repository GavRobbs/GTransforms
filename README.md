# Transform.cpp

This was a practice project I used to help me understand linear transformations. Game engines like Godot and Unity use these in combination with their scene graphs to position objects. When you have a hierarchical structure, the transformation matrix of an object is generated from the combined transformation matrix of its parents plus its own local transformation matrix, and this is fed to the graphics engine as the model matrix (as in, the same model matrix that makes up model-view-projection).

$$
M_{local} = M_{translation} \times M_{rotation} \times M_{scale}
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

I've made use of quaternions to represent rotations, and they've simplified many operations. Rotations can be combined by quaternion multiplication, they are easy to invert with the quaternion conjugate etc. Most of all, they are easy to convert to matrices and quaternion combination avoids the probelm of Gimbal lock.

The GLM library was used so that I didn't have to rewrite my own 3d math suite, and it has some useful functions, such as glm::to_string that saved me a lot of time.

## Design Inspiration

The primary design inspiration for this came from how Unity structures its Transform class. Mine isn't as feature complete, but I think I've learnt what I need to from it. I may extend it in the future if/when I build my own scene graph (likely with OpenGL).

## Potential Improvements

- A dirty flag for optimization purposes, to prevent recalculation of the global transformation matrix or basis vectors every time one is requested
- Some unit tests
- Convert it to a library instead of an executable