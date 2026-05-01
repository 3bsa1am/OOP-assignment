# OOP Assignment — C++ Image Editor (Console)

A C++ console application for a university **Object-Oriented Programming** assignment.

It loads an image, applies a set of filters/operations (grayscale, crop, blur, resize, etc.), supports **undo**, then saves the result.

> **Note**: This repository contains the assignment solution source code. The image I/O and `Image` implementation are provided through `Image_Class.h` (expected to be supplied by the course/assignment starter files).

## Features

The program presents a menu-driven interface and supports:

- **Load / Save** image
- **Undo** last operation (stack-based)
- Filters / operations:
  - Grayscale
  - Black & White (threshold)
  - Invert
  - Rotate (90/180/270/360)
  - Brightness (lighten/darken by percentage)
  - Crop
  - Blur (weak/medium/strong)
  - Purple (bonus)
  - Edge detection
  - Flip (horizontal/vertical)
  - Resize
  - Add frame (simple / double color / fancy corners)
  - Infrared (bonus)
  - Sunlight
  - Merge two images (averaging pixels; optionally resize to match)

Supported input/output extensions checked by the program:

- `.jpg`, `.jpeg`, `.png`, `.bmp`, `.tga`

## Repository contents

- `CS213_A1_S30_20242296_20240092_20240256.cpp` — main application and filter implementations
- `README.md` — this file

## Requirements

- A C++17 compatible compiler (GCC / Clang / MSVC)
- The course-provided image library header/source (referenced as `Image_Class.h`)
  - This project expects an `Image` class with at least:
    - `Image(std::string filename)` constructor
    - `saveImage(std::string filename)`
    - pixel access via `img(x, y, channel)` and/or `getPixel` / `setPixel`

## Build

### Linux / macOS (g++)

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -o oop_image_editor CS213_A1_S30_20242296_20240092_20240256.cpp
./oop_image_editor
```

### Windows (MSVC Developer Command Prompt)

```bat
cl /std:c++17 /EHsc CS213_A1_S30_20242296_20240092_20240256.cpp
CS213_A1_S30_20242296_20240092_20240256.exe
```

If your `Image_Class.h` requires linking additional `.cpp` files (common in course starter packs), include them in the compile command.

## Usage

1. Run the program.
2. Choose **1) load** and enter an image filename (with extension).
3. Choose any operation from the menu.
4. Choose **17) save** to write the output image.
5. Choose **18) undo** to revert one step.
6. Choose **19) exit**.

### Example session

```text
1.load
2.grey
3.black and white
...
17.save
18.undo
19.exit

file: input.jpg
2
17
same name? y/n: n
the new name: output.png
```

## Notes / Limitations

- **Undo** stores full image copies on a stack; very large images may use significant memory.
- Some operations assume valid user input (e.g., crop coordinates/dimensions).
- The project is menu-based and runs in the terminal; there is no GUI.

## Team

As listed in the source header comment:

- Mohammed Abdelsalam (20242296)
- Amir Mostafa (20240092)
- Sayed Mamdouh (20240256)

## License

No license file is currently included in this repository. If you plan to reuse or publish this project beyond coursework, consider adding a license (e.g., MIT) and confirming you are allowed to redistribute any course-provided image library code.
