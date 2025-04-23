# Project3
Running this code requires the user to have SFML 3.0, Boost, and ImageMagick installed.

SFML can be installed via the SFML website:
https://www.sfml-dev.org/download/sfml/3.0.0/   
For macOS users, SFML can be installed via homebrew using this terminal line:   
brew install sfml

Boost can be installed via the Boost website:
https://www.boost.org/users/download/   
For macOS users, Boost can be installed via homebrew using this terminal line:  
brew install boost

ImageMagick can be installed via the ImageMagick website:
https://imagemagick.org/script/download.php   
For macOS users, ImageMagick can be installed via homebrew using this terminal line:  
brew install imagemagick

Additionally, the user must download the following chess puzzle database: https://database.lichess.org/#puzzles     
The file comes in a compressed format, which can be decompressed differently depending on OS

Windows:    
Right clicking the file and extracting via 7Zip should decompress the file and return the "lichess_db_puzzle.csv" file

macOS:  
zstd can be installed via homebrew (in terminal enter "brew install zstd")   
To decompress the file, run this in terminal:   
zstd --decompress path/to/file.zst  
This will return the "lichess_db_puzzle.csv" file

The .csv file can then be placed in Project3/data/

To run the program, run the Main configuration and the user interface will prompt usage on solving chess puzzle using the different algorithms.
