## Stracer - Orestis Ro Stelmach BSc Thesis
---
#### Thesis title: Implementation of Parallel real-time ray-tracing algorithm 

A parallel progressive path-tracer written in Vulkan API, executing exclusively in Compute Shaders.
The image is generated as floating-point images in a "ping-pong" arrangement, which then then
result is transfered to fragment shader via texture, where it finally rendered to the screen.

The scene is loaded dynamically through JSON files.

Some features I would like to add in the near features are:
* Monte Carlo Integration
* Russian rulette path termination
* Spatial acceleration data structures
* Mesh loading
* Triangle intersection
* Better anti-aliasing

![raytraced img](/Screenshots/Screenshot10.png)
![raytraced img](/Screenshots/Screenshot3.png)
![raytraced img](/Screenshots/Screenshot1.png)
![raytraced img](/Screenshots/Screenshot2.png)
![raytraced img](/Screenshots/Screenshot4.png)
![raytraced img](/Screenshots/Screenshot5.png)
![raytraced img](/Screenshots/Screenshot6.png)
![raytraced img](/Screenshots/Screenshot7.png)
![raytraced img](/Screenshots/Screenshot8.png)
![raytraced img](/Screenshots/Screenshot9.png)

