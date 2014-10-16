# osk

Osk is a flexible and customizable on-screen keyboard for Windows supporting Unicode input and dynamic layout switching. It is designed to be used primarily on self-service terminals or other environments where only limited keyboard input is allowed. Osk simulates keystrokes using [SendInput](http://msdn.microsoft.com/en-us/library/windows/desktop/ms646310%28v=vs.85%29.aspx) call.

## Build Requirements
 - Qt 4.8 
 - Windows SDK 7
 - Microsoft Visual C++ 2010 Express

## Usage

Osk is ran by invoking its executable. After start it reads the settings file and tries to load a default layout (en_US.json) or the last used one (if the corresponding option is set) from the predefined directory `layouts`. Based on settings, Osk is either
 - docked at screen bottom or
 - docked at screen top or
 - floating at fixed position with fixed geometry.
 
Osk is designed to prevent accidental closing, so there's no way to exit the application except sending the TERM signal using taskkill, for example.

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
 - `right` -- sends the right Arrow key code

```javascript
{
    "role": string,
    
    "main": string or integer, // only for 'char' role
    "shift": string or integer  // only for 'char' role
}
```


####

### Rows

Key definitions are organized in `rows` 
 
### Layout

Rows are organized in the root object.

The root object must contain `rows` Array. Each item of `rows` array describes a line of keys on the keyboard. The root object may also contain integer `colSpan`, `rowSpan` and a boolean `showShift` items. If defined, these values are applied to all the keys on the keyboard unless on the nested levels.

```javascript
{
    "colSpan": 2,    // default colSpan applied to all the keys in keyboard
    
    "rows": 
}
```

