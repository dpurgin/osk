# osk

Osk is a flexible and customizable on-screen keyboard for Windows supporting Unicode input and dynamic layout switching. It is designed to be used primarily on self-service terminals or in any other environment where only limited keyboard input is allowed. Osk simulates keystrokes using [SendInput](http://msdn.microsoft.com/en-us/library/windows/desktop/ms646310%28v=vs.85%29.aspx) call.

**Table of Contents**

- [osk](#user-content-osk)
	- [Build Requirements](#user-content-build-requirements)
	- [Usage](#user-content-usage)
	- [Settings](#user-content-settings)
		- [Docking and Keyboard Geometry](#user-content-docking-and-keyboard-geometry)
	- [Editing Layouts](#user-content-editing-layouts)
		- [Keys](#user-content-keys)
			- [Character Key Example](#user-content-character-key-example)
			- [Function Key Example](#user-content-function-key-example)
			- [Loadlayout Key Example](#user-content-loadlayout-key-example)
		- [Rows](#user-content-rows)
		- [Shift Mode](#user-content-shift-mode)
	- [Styling](#user-content-styling)
		- [Keyboard Style](#user-content-keyboard-style)
		- [Key Style](#user-content-key-style)
		- [Styling Key Labels](#user-content-styling-key-labels)
			
## Build Requirements
 - Jansson (referenced in repository)
 - Qt 4.8 
 - Windows SDK 7
 - Microsoft Visual C++ 2010 Express

## Usage

Osk is ran by invoking its executable. After start it reads the settings file and tries to load a default layout (en_US.json) or the last used one (if the corresponding option is set) from the predefined directory `layouts`. Based on settings, Osk is either
 - docked at screen bottom or
 - docked at screen top or
 - floating at fixed position with fixed geometry.
 
Osk is designed to prevent accidental closing, so there's no way to exit the application except sending the TERM signal using taskkill, for example.

## Settings

Settings are stored in `osk.ini` file at the same level as the executable. The settings file is explained below:

```
[layout]
openLastLayout=false            # put this to true if you want the application to load the last layout used upon start
lastLayout=en_US.json           # the application updates this field whenever a user changes layout
defaultLayout=en_US.json        # reasonable default layout to start the application with

[position]
dockPosition=bottom             # the application will be docked to the bottom of the screen. 
left=0                          # not used while docking, specifies upper left corner upon start otherwise
top=0                           # not used while docking, specifies upper left corner upon start otherwise
width=1080                      # not used while docking, specifies keyboard width otherwise
height=350                      # keyboard height while docking or floating mode

[style]
stylesheet=flat.css             # reads and sets styles/flat.css as stylesheet
```

### Docking and Keyboard Geometry

The `dockPosition` option takes on of three values:

 - `bottom` - the keyboard is shown at the bottom of the screen with `height` property of the settings, screen width;
 - `top` - the keyboard is shown at the top of the screen with `height` property of the settings, screen width;
 - `none` - the keyboard is shown at `(left, top`) position with `width` and `height` of the settings.

In any case, the keyboard can not be moved by user.

## Editing Layouts

Keyboard layout definitions must reside in the predefined `layouts` directory at the same level with osk's executable.

A layout is defined in a plain UTF-8 encoded text file (no BOM). It contains a JSON object describing keys, their roles, position on the keyboard and display properties. Keys are layed out in a grid, so column and row spanning may apply.

### Keys 

Each key must be assigned its specific role. This affects the definition of key. Roles are:
 - `char` -- inputs a Unicode character defined as an integer code or a character;
 - `backspace`-- sends the Backspace key code;
 - `tab` -- sends the Tab key code;
 - `enter` -- sends the Enter key code;
 - `capslock` -- toggles Shift mode until pressed again;
 - `shift` -- toggles Shift mode for one keystroke;
 - `space` -- inputs a whitespace character;
 - `up` -- sends the Up Array key code;
 - `down` -- sends the Down Arrow key code;
 - `left` -- sends the Left Arrow key code;
 - `right` -- sends the right Arrow key code;
 - `loadlayout` -- removes current layout and loads the new one.
 
 Key definition can contain the following properties:

```JSON
{
    "role": string,   // a role from the list above
    
    "col": integer, // optional, column to put the key to
    "colSpan": integer, // optional, column spanning
    "rowSpan": integer, // optional, row spanning
    
    "main": string or integer, // 'char' role only
    "shift": string or integer,  // optional, 'char' role only, represents input in combination with Shift
    "showShift": boolean, // optional, 'char' role only, render alternative character on the key
    
    "title": string, // 'loadlayout' role only, text to display on button
    "layout": string, // 'loadlayout' role only, file name of a layout to load relative to 'layouts' directory
}
```

#### Character Key Example 

The following example defines a key that sends '1' as its primary input, '!' as input in Shift mode. Both primary and shift mode characters are rendered on the key.

```JSON
{
    "main": "1",
    "shift": "!",
    "showShift": true
}
```

The following example defines a key that sends lowercase 'z' as its primary input, capital 'Z' as its Shift mode input. Only the current mode input is rendered on the key.

```JSON
{
    "main": "z",
    "shift": "Z",
    "showShift": false
}
```

#### Function Key Example

The following example defines a Tab key. It is spanning 3 columns which would make it wider than the character keys with `colSpan: 2`.

```JSON
{
    "role": "tab",
    "colSpan": 3
}
```

#### Loadlayout Key Example

The following example defines a key with "Deu" text on it that loads German layout from `layouts/de_DE.json`.

```JSON
{
    "role": "loadlayout",
    "layout": "de_DE.json",
    "title": "Deu"
}
```

### Rows

Each set of key definitions that has to appear as a row on the keyboard should be put in a JSON Array of Objects defining the keys in the desired order. Osk reads the array and lays out the keys in the same order they appear. The array of key definitions has to be put into `items` property of a keyboard row definition. Row definition is a JSON Object contatining the following properties: 

```JSON
{
    "colSpan": integer, // optional, default column spanning for all items
    "rowSpan": integer, // optional, default row spanning for all items
    "showShift": boolean, // optional, default value for rendering Shift mode character for all items
    
    "items": array   // mandatory, array of key definitions (see above)
}
```

`colSpan`, `rowSpan` and `showShift` properties specified in row definition are used for all the keys in this row unless overriden by the same property in key definition. Consider the following example of row definition:

```JSON
{
    "showShift": false,    // shift mode characters are not rendered for all keys in this row
    
    "items": [{
        "main": "j",
        "shift": "J"
    }, {
        "main": "k",
        "shift": "K"
    }, {
        "main": "l",
        "shift": "L"
    }, {
        "main": ";",
        "shift": "\"",
        "showShift": true  // shift mode character is rendered for this key only
    }]
}
```

Empty space can be created by explicitly specifiyng `col` while defining a key. Consider the following example of creating a numpad: 

```JSON
{
    "colSpan": 2,
    "showShift": true
    
    "items": [{
        "main": "-",     // layed out in position 1, colspan 2
        "shift": "_"
    }, {
        "main": "=",     // layed out in position 3, colspan 2
        "shift": "+"
    }, {
        "role": "backspace",   // layed out in position 5, twice wider than the character buttons
        "colSpan": 4
    }, {
        "main": "7",      // would lay out in position 9 but explicitly set to col 10, hence creating empty space
        "col": 10         // between "main" keyboard and numpad
    }]
}
```
 
Rows are organized in `rows` Array of the root object in the same manner as keys are organized in a row. The root object may also contain integer `colSpan`, `rowSpan` and a boolean `showShift` values. If defined, these values are applied to all the keys on the keyboard unless explicitly overriden for a row or a key.

### Shift Mode

To input the characters defined in `shift` property of key definitions, the keyboard must go into Shift mode. This can be done by pressing a button with either `shift` or `capslock` role. `Shift` key puts the keyboard into the Shift mode for one keystroke, while `capslock` key does this for unlimited keystrokes until the `capslock` button isn't hit again. Being pressed at the same time, `capslock` and `shift` button do effectively cancel each other, hence moving the keyboard back into Main mode again. 

## Styling

Stylesheets are residing in `styles` directory at the same level as the executable. There are two styles shipped with the program: `flat.css` imitating Windows 8 flat keyboard and `3d.css` somehow resembling 3D look of Windows 7 keyboard. Stylesheets are created and modified according to [Qt 4 stylesheet](http://qt-project.org/doc/qt-4.8/stylesheet.html) reference manual with an extension of OSK explained further.

### Keyboard Style

Keyboard itself (container holding the keys) is a QWidget descendant, so it is styled as a QWidget:

```CSS
QWidget {
    background-color: gray;
}
```

### Key Style

Key backgrounds (containers holding text) are QLabels, so they should be styles as a QLabel:

```CSS
QLabel {
    font-family: 'Segoe UI Light';

    background-color: black;
}
```

A pressed key holds `pressed=true` dynamic property:

```CSS
QLabel[pressed="true"] {
    background-color: #A0A0A0;
}
```

### Styling Key Labels

*This part is Osk's extension to Qt 4 stylesheets*.

Key text styles are like CSS class selectors. Primary key text styles as .key-primary: 

```CSS
.key-primary {
    font-size: 32px;
    color: white;
}
```

Secondary key text (the one produced by a key in Shift mode) is styled as .key-secondary:

```CSS
.key-secondary {
    font-size: 24px;
    color: #A0A0A0;
}
```

Pressed key texts are styled with :active selector: 

```CSS
.key-primary:active {
    font-size: 32px;
    color: white;
}
```

Note that the active selector still requires all style properties set, it behaves like a separate style and does not override any property of the same class without :active selector. *This behavior differs from standard CSS.*
