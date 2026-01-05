# Example use of GUICreator

### **Quick Info**

This is a small taco truck cash register app to test out the GUICreator package

I use a 'home made' package manager that will be abbreviated as PPM which I may or may not push to GitHub

To get going use PPM and add all the external packages listed below.  All required 'home-made' packages have already been copied into project

Some scaling happens on either the screen width or height depending on which is smaller.  It will be known as base-size

My home made packages have been made over a range of time, caffeine levels, and happiness so be warned that the quality varies.

### **Page Info**

Each page is an xml file that describes the initial placement of the elements

Adding more elements dynamically is possible, but not supported in the GUICreator used when making this

The name of each element does not mean anything, just used to help write out what it does

Positioning of child objects is done on a grid basis by default

General attributes can be applied to root node

Default values are bound to change, so to test page just run and see what you want changed/enabled/disabled/whatever

Functions are set with a space ( *namespace* ) and function pointer.  Pages will reference the function by the name set.  With no space specificed it searches in the space with the corresponding name.  To specifiy a specific space do {space:function}

**Types**

| Type | Expected input |
|------|----------------|
| vec4 | {x,y,z,w} |
| vec2 | {x,y} |
| bool | [true/false] |
| string | {string} |
| int | {int} |
| float | {float} |

**General attributes**

| Type | Name | Description |
|------|------|-------------|
| vec4 | primaryColor | Background color or primary in gradient |
| vec4 | secondaryColor | Secondary color in gradient |
| vec4 | borderColor | Border color |
| vec2 | gradientStart | Where the gradient will start in the object on a [-1,-1] - [1,1] range within the element |
| bool | gradientX | Enables gradient on x-axis |
| bool | gradientY | Enables gradient on y-axis |
| bool | manhattanGradient | Use manhattan distance for gradient instead of linear |
| bool | border | Enables border |
| float | borderThickness | Changes thickness of border [0-2] range based on base-size |
| float | gradientStep | Changes the gradient step size based on element size, or 0 for none |
| float | gradientDistance | The distance away from 'gradientStart' to be at 'secondaryColor' 100% |
| string | image | The image to use in the background, disables colored background |

**Element positioning attributes**

| Type | Name | Description |
|------|------|-------------|
| int | gridX | X-size of grid |
| int | gridY | Y-size of grid |
| float | scaleX | X-scale |
| float | scaleY | Y-scale |
| float | offsetX | X-offset |
| float | offsetY | Y-offset |
| bool | noMove | Stops parent-grid position from incrementing with this object |
| bool | noGrid | Doesn't use parent-grid, gets same transform as parent |
| int | jumpGridX | Preincrements parent-x-grid position by amount |
| int | jumpGridY | Preincrements parent-y-grid position by amount |
| vec2 | padding | Amount to keep child elements and text away from borders, combined between both sides  |

**Element attributes**

| Type | Name | Description |
|------|------|-------------|
| string | id | The object ID to search for it with 'GetObjectByID', "base" is use exclusively for root object |
| bool | stick | Sticks the element's position relative to the screen border |
| bool | stretch | Stretches the element's size as screen size changes |
| bool | stuck | Sticks the element's position relative to placement on 1:1 screen |
| bool | centerTextX | Centers text horizontally |
| bool | centerTextY | centers text vertically |
| bool | enabled | Enables drawing for entire sub-tree |
| float | textSize | Size of text |
| string | onClick | Location of function for an onClick callback |
| string | onRelease | Location of function for an onRelease callback |
| string | onEnter | Location of function for an onEnter callback |
| string | onLeave | Location of function for an onLeave callback |


### **Packages used**
**Home made packages**
- animateValue
- guiCreator
- shaderHandling
- textRendering
- timer
- vector
- windowManager

**External packages**
- glad
- glfw
- glm
- freetype
- stb-master
- sqlite