# Configuration Files and Constants
Constants and default values (Such as the game title or version) are defined in configuration headers. These are files 
ending with _config.h stored in src/config/ that contain a namespace with relevant constants and attributes.

For string constants, std::string_view is preferable to std::string because it's a read only pointer in static memory. 
(std::string cannot be set to constexpr)

# Presentation Modes
Free:
Reference resolution is ignored, the window displays as much of the content as it can with no scalaing. 
Standard option for UI applications.

Letterbox:
The content inside the reference resolution is scaled uniformly (Maintaining aspect ratio) to fit the window, centered. 
The remainder of the window is filled with a border colour (Black by default)

Crop:
The content inside the reference resolution is scaled uniformly (Maintaining aspect ratio) to fit into the smallest axis
of the window, centered. This process leaves to gaps and usually results in cropping parat of the reference resolution.

Stretch:
The content inside the reference resolution is stretched to match the size of the window without maintaining aspect 
ratio. The content will be distorted unless the aspect ratio is exactly the same as the reference resolution's.

Expand:
The content inside the reference resolution is scaled uniformly (Maintaining aspect ratio) to fit the window, centered.
The remainder of the window reveals content to the right/left or top/bottom of the screen.

ExpandHorizontal:
The content inside the reference resolution is scaled uniformly (Maintaining aspect ratio) to fit the height of the 
window, centered.
The remainder of the window reveals content to the right/left.

ExpandVertical:
The content inside the reference resolution is scaled uniformly (Maintaining aspect ratio) to fit the width of the 
window, centered.
The remainder of the window reveals content to the top/bottom.
