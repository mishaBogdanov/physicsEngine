# In Terminal Physics Engine:  

by Mikhail Bogdanov       
read me created on May 14th 2024  
demo: https://www.youtube.com/watch?v=V_mtm3N9M-0   


---  

![image](https://github.com/mishaBogdanov/physicsEngine/assets/78181954/af5b7e19-1d15-4105-a1eb-1bf6d7164146)
# background:
I started this project mid january after classes concluded, as a way to take my mind of studying for finals. Initially I created a software renderer to render a spinning cube in terminal, which only used the CPU. I thought it looked like a unique art style to make a full game engine in, and was inspired by "Code-It-Yourself! First Person Shooter (Quick and Simple C++)" by javid9X on YouTube, who made a simple first person game in terminal. However, after experimenting with adding hundreds of triangles to my scene, I quickly began to notice the speed of CPU rendering decreasing substantually. After learning the basics of openGL, I created a simple rendering engine, the details of which I will put in the specifications below. I hid the window output from openGL, stored all the pixels in an array, and changed them to characters 1 by 1, depending on the sum of the red green and blue channels from the pixel. After this I made my own .obj importer to make it possible to import 3D models I downloaded of the internet. Next I began working on the physics engine, but quickly came to the conclusion I had no idea how to implement rigid body physics, since all I had is a simple mechanics course from first year UBC, and at the time I was planning on taking advanced mechanics in 3rd year. This forced me to seek resources online, where I quickly hit a brick wall, realizing that there is a lot more to basic rigid body physics simulation than I initially expected. I spent the next couple weeks reading and studying "Game Physics Engine Development" by Ian Millington, as well as some other online resources, from which I began getting a grasp on what I had to do, to accomplish just a fraction of what I was initially intending.

# features/specifications/methods:
rendering/visuals:  
- renders everything using 2 shaders.
- shader 1: includes a vertex, geometry and fragment shader. the geometry shader is responsible for calculating the dot product between the face normal, and the way the player is facing to then pass a color into the fragment shader. This produces the effect of making the triangles facing your direction brighter, while making the ones angled diagonally dimmer. This is how I give objects depth, so they can actually be visible in 3D.
- shader 2: includes a vertex, geometry and fragment shader, the geometry shader is used to once again calculate the face normal, and then lift an outline of the triangle by a tiny fraction of a unit, and draws a line outlining the triangle. this is done to improve clarity, and in my opinion makes 3D objects easier to distinguish in the low resolution output of the terminal.
- object imports are done with a custom script I wrote. One downside of my current setup is that each object has to be triangulated before importing into the engine.
- seperate shader is used for the floor which outlines the triangles and paints them black.
physics implementation:
- types of objects that can be imported, each of which has adjustuble position and scale:
  - no physics: a model is imported as a simple model which has no effect on the objects around it. is pretty much a ghost.
  - a movable object with a hitbox: imports an object with an invisible hitbox, which is automatically effected by gravity (gravity can be turned off). Will collide and bounce off of other objects with hitboxes. Hitboxes are generated like a cube which is the size of the max/min positions in the x/y/z directions.
  - a hitbox only mode: is designed to make map creation easier. Allows you to import a set of cubes which act as independent, imovable and invisible hitboxes in the world. that's how I generated the hitboxes on the Dust 2 map, by going in blender and putting in boxes inside the walls of a downloaded dust 2 replica. I then imported the dust 2 map as a no physics model, and imported the hitboxesa in the hitbox only mode. This allowed me to not bother having to manualy put hitboxes inside my engine, but rather inside blender.
  - an imovable object with a hitbox: this is how I make the floor and the ramp in the demonstration video. these objects are unable to move, and will bounce any other (movable) object of its surface.
- collision detection is done through separating axis theorem
- all the objects detected to be colliding than project the maximums and minimums of their hitboxes onto eachother to generate the contact points.
- I then use a rigidbody formula for generating the apropariate impulses on all the points of contact, and divides each of them by the number of points of contact.
- The engine supports multiple itiration solution, however for my showcase I found it sufficient to just use 1 iteration.
- after applying the impulses, it goes over all the points of contact, and applies a friction force that is proportional to the collision impulse. it then checks if the relative velocities of the 2 points are still in the same direction, and if not cancels the aplication of the force.

# Future plans:
- implement a more general constraint solver system. This would require me to generate even more contact points, and require a lot more research, however the benefits would be the possibility of joints, pivots, engines with torque, suspension, and better friction, with both static and dynamic friction handling.
- make a more advanced collision detection system, using trees and distance comparisons rather than just separating axis theorem.
- clean up the code, and add more comments explaining the purpose of functions, to make the code more readable.
- add an option to render to a window with graphics.


# if I were to do it again:
- implement a robust debugging view. It was very challenging to figure out my math mistakes and general bugs without a way to see exactly what's happening, and due to the random nature of a physics engine, with random frame timimng, it was hard to reproduce results to test out changes. I think having a start option to run in a normal window, as well as easily place markers at contact points and a 3D vector visualization at impulse points would've helped a LOT with the debugging process.
- separate out the physics from the rest of the code. Right now I have it setup such that the physics code is in the world code, and I think it would make the code more readable and easy to follow if physics was part of its own class. 


