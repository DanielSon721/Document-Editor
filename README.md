# Simple Document Editor in C

This project is a lightweight document editor written in C that allows for creating, modifying, and saving structured documents composed of paragraphs and lines. It supports operations such as inserting, deleting, highlighting, and replacing text, as well as reading from and writing to files.

## Features

- Create and initialize new documents
- Add or remove paragraphs and lines
- Replace, highlight, or remove text within the document
- Load document contents from an array or file
- Save document contents to a file
- Print document to standard output

## File Structure

- `document.c` – Main implementation file
- `document.h` – Header file (assumed to include type definitions and constants)
- Other headers: `<stdio.h>`, `<string.h>`

## Example Operations

- `add_line_after(&doc, 1, 0, "Hello World");` – Add a line after line 0 in paragraph 1  
- `replace_text(&doc, "World", "Universe");` – Replace "World" with "Universe"  
- `highlight_text(&doc, "error");` – Highlight the word "error" (→ `[error]`)  
- `save_document(&doc, "output.txt");` – Save document to file  

## Saving & Loading

- `load_file` loads a document from a plain text file. Blank lines are treated as paragraph separators.
- `save_document` saves the document to a plain text file, preserving paragraph structure.

## Notes

- Max string size per line: `MAX_STR_SIZE`
- Max lines per paragraph: `MAX_PARAGRAPH_LINES`
- Max number of paragraphs: `MAX_PARAGRAPHS`
- All constants should be defined in `document.h`.

## Author

Daniel Son  
Student ID: 119710265  
GitHub: [djson721](https://github.com/djson721)
