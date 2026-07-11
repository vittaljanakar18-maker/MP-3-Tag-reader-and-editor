# MP3 Tag Reader and Editor

## Overview

MP3 Tag Reader and Editor is a command-line application developed in C that allows users to read and modify ID3 metadata stored in MP3 files. The application extracts information such as song title, artist, album, year, genre, and comments, and also provides the ability to edit these tags efficiently using binary file operations.

---

## Features

- View MP3 ID3 tag information
- Edit song title
- Edit artist name
- Edit album name
- Edit year
- Edit genre
- Edit comments
- Command-line interface
- Efficient binary file handling

---

## Technologies Used

- C Programming
- File Handling
- Binary File Operations
- Structures
- Command Line Arguments
- Modular Programming

---

## Project Structure

```
MP3-Tag-Reader-and-Editor/
│── main.c
│── view.c
│── view.h
│── edit.c
│── edit.h
│── types.h
│── sample.mp3
│── README.md
```

---

## Compilation

```bash
gcc *.c -o mp3tag
```

---

## Usage

### View MP3 Tags

```bash
./mp3tag -v sample.mp3
```

### Edit Title

```bash
./mp3tag -e -t "New Title" sample.mp3
```

### Edit Artist

```bash
./mp3tag -e -a "Artist Name" sample.mp3
```

### Edit Album

```bash
./mp3tag -e -A "Album Name" sample.mp3
```

### Edit Year

```bash
./mp3tag -e -y "2025" sample.mp3
```

### Edit Genre

```bash
./mp3tag -e -g "Pop" sample.mp3
```

### Edit Comments

```bash
./mp3tag -e -c "New Comment" sample.mp3
```

---

## Learning Outcomes

- Binary file handling in C
- ID3 tag manipulation
- File pointer operations
- Structures and modular programming
- Command-line argument processing

---

## Future Enhancements

- Support for ID3v2 tags
- Batch editing multiple MP3 files
- Playlist support
- Graphical User Interface (GUI)
- Support for additional audio formats

---

## Author

**Vittal Janakar**

Electronics and Telecommunication Engineering  
Dr. Ambedkar Institute of Technology
