/// **********************************************************************************************
/// Music library - this C++ program displays the database of details of all the albums provided. 
///                  Also, checks whether a song, album, or artist name exits in the dataset
///                  and generates states and/or album details when prompted. 
/// libraries added - map, set, fstream, sstream
/// Student name: Srijita Banerjee          
/// netid: sbane4@uic.edu, UIN: 675443340
/// **********************************************************************************************
/// Assignment details and provided code are created and
/// owned by Adam T Koehler, PhD - Copyright 2023.
/// University of Illinois Chicago - CS 251 Spring 2023

#include <iostream> // for input/output
#include <string> // for string handling
#include <set>  // primary data structure for this program 
#include <fstream> // for file handling
#include <sstream> // for stringstream (just in case!)
#include <ctype.h> //for isdigit function
#include <tuple> // for tie (in operator overloading)
#include <ostream> //for output file stream (required for exporting album details)
#include <algorithm> // for set operations


using namespace std;

// INSERTS GIVEN HELPERS CODE BELOW - DO NOT REMOVE
#include "helpers.cpp"
// INSERTS GIVEN HELPERS CODE ABOVE - DO NOT REMOVE

//
// NOTE TO STUDENTS:
// You may use the functions in the helpers file,
// but you may not modify the file or its contents in any way whatsoever.
//

//
// STUDENT CODE MAY GO BELOW THIS LINE
//



// creating a struct with album name, artist name(s), and set of song(s)
struct album_details {
    string album;
    set <string> artist_names;
    set <string> album_songs;

}; 

// Operator overloading: operator<
inline bool operator< (const album_details &lhs, const album_details &rhs) {
  return tie (lhs.album, lhs.artist_names, lhs.album_songs) < tie (rhs.album, rhs.artist_names, rhs.album_songs);
}



// This function checks whether the sentence has a number or not
bool line_starts_with_number (string sentence) {
    char first = sentence[0];
    char second = sentence[1];
    char third = sentence[2];
    if (isdigit (first) && isdigit (second) && isspace(third)) {
        return true;
    }
    return false;
}

// This function loads the album name, artist name, and all their songs in the nested map
void load ( set <album_details> &data, string filename) {

    album_details load_info;
    string current_album = "", album = "",  song = "", line = "", artist = "";
    if (filename == " ") {
        filename = "musicdatabase.txt"; // if no file name is provided, musiclibrary.txt is to be loaded
    }

    fstream infile (filename);
    
    if (infile.good ()) {
        getline (infile, current_album); // getting the name of the first album in the text file
        while (!infile.eof ()) {
            getline (infile, line);
            // cout << "line:" << line << endl;
            // if the sentence (or, line) starts with a number, that's a song name
            if (line_starts_with_number (line)) {
                load_info.album_songs.insert(line); // getting name of song(s)
            } else {
                if (line != current_album) {
                    load_info.artist_names.insert (line); // getting the artist name(s)
                    
                } else {
                    // cout << line << ":" << current_album << endl;
                    load_info.album = line; // getting the name of the album
                    getline (infile,current_album); // getting the name of the next album
                    data.insert(load_info); // creating a set of structs of album details
                    load_info.album_songs.clear(); // resetting set ot songs
                    load_info.artist_names.clear(); // resetting set of artists
                }
                
            }
        }

    } else {
        cout << "Error: Could not open music library file - " << filename << endl;
    }
    return;
}




// This function calculates and displays the total number of albums, unique artists, and songs present in the set
void generate_stats (set <album_details> &data) {
    album_details stats_info;

    set <string> unique_songs;
    set <string> unique_artists;
    int total_songs = 0, total_artists = 0, total_albums = 0;

    for (auto it = data.begin(); it != data.end (); ++it) {
       total_albums++; // total number of albums
       // cout << it->album << endl; // FIXME: DELETE IT

       for (auto j = it->album_songs.begin(); j!=it->album_songs.end(); ++j) {
        unique_songs.insert(*j); // inserting unique songs to this set
        }

        for (auto itr = it->artist_names.begin(); itr!=it->artist_names.end(); ++itr) {
            unique_artists.insert(*itr); // inserting names of unique artists to this set
        }
    }

    total_songs = unique_songs.size(); // total number of unique songs counted
    total_artists = unique_artists.size(); // total number of unique artists counted

    // Printing stats in the format provided
    cout << "Overall Music Library Stats" << endl
         << "===========================" << endl
         << "Total Records: " << total_albums << endl
         << "Total Unique Artists: " << total_artists << endl
         << "Total Songs: " << total_songs << endl;
         

    return;
}

// This function generates the details of all albums present in the data structure
void generate_stats_and_details (set <album_details> &data) {

    // calling generate_stats() first
    generate_stats(data);

    // printing details of albums present in data structure
    cout << endl
         << "Your Current Music Library Includes" << endl
         << "===================================" << endl;
    
    for (auto i = data.begin(); i != data.end(); ++i) {
        cout << i->album << endl; // rinting album name

        for (auto j = i->artist_names.begin(); j != i->artist_names.end (); ++j) {
            cout << " " << *j << endl; // printing names of artist(s) 
        }

        for (auto itr = i->album_songs.begin (); itr != i->album_songs.end (); ++itr) {
            cout << "   " << *itr << endl; // printing song names
        }
        
    }
    cout << endl;
    return;
}


// this function exports the contents of the set into a text file
void export_album_details (set <album_details> &data, string filename) {
    ofstream onfile (filename);
    for (auto i = data.begin (); i != data.end (); ++i) {
        onfile << i->album << endl; // writing album name before adding more album details

        for (auto j = i->artist_names.begin (); j != i->artist_names.end (); ++j) {
            onfile << *j << endl; // writing artist(s) names in the text file
        }

        for (auto itr = i->album_songs.begin (); itr != i->album_songs.end (); ++itr) {
            onfile << *itr << endl; // writing song names in the text file
        }

        onfile << i->album << endl; // writing album name after adding all album details
    }
    return;
}

// This function implements search functionality when an album (or albums) is to be searched from the set
void search_for_album (set <album_details> &data, set <string> &search_items) {

    string word = "", each_album = "";
    set <string> found_in_album;
    set <string> resultant_set; // for union/intersection/difference

    for (auto i=search_items.begin(); i!=search_items.end(); ++i) {
        word = *i;
        if (word[0] == '+' || word[0] == '-') {
            word = word.substr(1); // if the word starts with a modifier, eliminate it for now, for later comparison
        }
        for (auto j=data.begin (); j!=data.end (); ++j) {
            each_album = j->album;
            
            tolower(each_album); 
            if (each_album.find(word) != string::npos) {
                found_in_album.insert (j->album); // inserting album names that match the search criteria
            }

        }
        if ((*i)[0]!='+' && (*i)[0]!='-') { // considering the case of NO modifier: search album string1, string2
            set_union (begin (found_in_album), end (found_in_album),
                       begin (resultant_set), end (resultant_set),
                       inserter (resultant_set, end (resultant_set)));
                
        } else if ((*i)[0]=='+') {
               set_intersection (begin (found_in_album), end (found_in_album),
                       begin (resultant_set), end (resultant_set),
                       inserter (resultant_set, end (resultant_set))); // considering the case of '+' modifier: search album string1, +string2, etc.
        } else if ((*i)[0]=='-') {
               set_difference (begin (resultant_set), end (resultant_set),
                       begin (found_in_album), end (found_in_album),
                       inserter (resultant_set, end (resultant_set))); // considering the case of '-' modifier: search album string1, -string2, etc.
        }
        found_in_album.clear(); // resetting the set of found albums
        
    }

    cout << "Your search results exist in the following albums: " << endl;

    for (auto i=resultant_set.begin(); i!=resultant_set.end(); ++i) {
        cout << *i << endl;
    }
    resultant_set.clear (); 
    cout << endl;
    
}

// This function implements search functionality when an artist (or artists) is to be searched from the set
void search_for_artists (set <album_details> &data, set <string> &search_items) {
    string word = "", each_artist = "";
    set <string> found_in_album;
    set <string> resultant_set; // for union/intersection/difference

    for (auto i=search_items.begin (); i!=search_items.end (); ++i) {
        word = *i;
        if (word[0]=='+' || word[0]=='-') {
            word = word.substr (1); // removing +/- from the search_item for now
        }
        for (auto j=data.begin (); j!=data.end(); ++j) {
            for (auto itr = j->artist_names.begin(); itr!=j->artist_names.end(); ++itr) {
                each_artist = *itr;
                tolower (each_artist); // converting the artist name to lowercase for convenience of comparison
                if (each_artist.find(word) != string::npos) {
                    found_in_album.insert(j->album); // inserting the album name that matches the search criteria
                }
            }
        }
        if ((*i)[0]!='+' && (*i)[0]!='-') { // considering the case of NO modifier: search album string1, string2
            set_union (begin (found_in_album), end (found_in_album),
                       begin (resultant_set), end (resultant_set),
                       inserter (resultant_set, end (resultant_set)));
                
        } else if ((*i)[0]=='+') {
               set_intersection (begin (found_in_album), end (found_in_album),
                       begin (resultant_set), end (resultant_set),
                       inserter (resultant_set, end (resultant_set))); // considering the case of '+' modifier: search album string1, +string2, etc.
        } else if ((*i)[0]=='-') {
               set_difference (begin (resultant_set), end (resultant_set),
                       begin (found_in_album), end (found_in_album),
                       inserter (resultant_set, end (resultant_set))); // considering the case of '-' modifier: search album string1, -string2, etc.
        }
        found_in_album.clear(); // resetting the set of found albums
    }

    cout << "Your search results exist in the following albums: " << endl;

    for (auto i=resultant_set.begin(); i!=resultant_set.end(); ++i) {
        cout << *i << endl;
    }
    
    cout << endl;
}

// This function implements search functionality when a song/(or songs) is to be searched from the set
void search_for_songs (set <album_details> &data, set <string> &search_items) {
    string word = "", each_song = "";
    set <string> found_in_album;
    set <string> resultant_set;

    for (auto i=search_items.begin(); i!=search_items.end(); ++i) {
        word = *i;
        if (word[0]=='+' || word[0]=='-') {
            word = word.substr(1);
        }

        for (auto j=data.begin(); j!=data.end(); ++j) {
            for (auto itr = j->album_songs.begin(); itr!=j->album_songs.end(); ++itr) {
                each_song = *itr;

                tolower (each_song);
                if (each_song.find (word)!=string::npos) {
                    found_in_album.insert(j->album);
                }
            }
        }
        if ((*i)[0]!='+' && (*i)[0]!='-') {
            set_union (begin (found_in_album), end (found_in_album),
                       begin (resultant_set), end (resultant_set),
                       inserter (resultant_set, end (resultant_set)));
        } else if ((*i)[0]=='+') {
            set_intersection (begin (found_in_album), end (found_in_album),
                       begin (resultant_set), end (resultant_set),
                       inserter (resultant_set, end (resultant_set)));
        } else if((*i)[0]=='-') {
            set_difference (begin (resultant_set), end (resultant_set),
                       begin (found_in_album), end (found_in_album),
                       inserter (resultant_set, end (resultant_set)));
        }
        found_in_album.clear ();
    }
    cout << "Your search results exist in the following albums: " << endl;
    for (auto i=resultant_set.begin(); i!=resultant_set.end(); ++i) {
        cout << *i << endl;
    }
    resultant_set.clear(); 
    cout << endl;
}


// driver function
int main ()
{
    string userEntry;
    string command, remains;

    // TODO: Declare any necessary variables for the music libary
    string no_filename = "", search_place = "", word = "";

    // the data structure I am using for this program: set of structs
    set <album_details> musicinformation;
    
   
    // Display welcome message once per program execution
    cout << "Welcome to the Music Library App" << endl;
    cout << "--------------------------------" << endl;

    // Stay in menu until exit command
    do
    {
        cout << endl;
        cout << "Enter a command (help for help): " << endl;
        getline(cin, userEntry);
        cout << endl;

        // Split the user entry into two pieces
        splitFirstWord(userEntry, command, remains);
        tolower(command);

        // take an action, one per iteration, based on the command
        if (command == "help")
        {
            helpCommand();
        }
        else if (command == "clear")
        {
            musicinformation.clear(); // clearing the contents of the set
        }
        else if (command == "export")
        {
           if (remains == "") {
            no_filename = "musicdatabase.txt";
            export_album_details (musicinformation, no_filename); // if no file name is provided, album details must be added in musicdatabase.txt
            } else {
                export_album_details (musicinformation, remains); 
            }
            
        }
        else if (command == "load")
        {
            // calling load function
            if (remains == "") {
               no_filename = "musicdatabase.txt";
               load (musicinformation, no_filename); // remains: name of the text file, sending a reference of set musiclibrary
                
            } else {
                load (musicinformation, remains);
            }
        }
        else if (command == "stats")
        {
            if (remains=="") {
                generate_stats(musicinformation);
                cout << endl;
            } else if (remains == "-d") {
                generate_stats_and_details (musicinformation);
                //cout << endl << endl; // FIXME: DELETE IT
            }
        }          
        else if (command == "search")
        {   
            if (remains=="") {
                cout << "Error: Search terms cannot be empty." << endl
                     << "No results found." << endl
                     << endl;
            } else {
                stringstream ss (remains);
                ss >> search_place; // extracted the thing that needs to be searched
                set <string> search_items;
                while (ss >> word) {
                        tolower (word);// converting the string to lowercase
                        search_items.insert (word); // extracted all words to be searched from the dataset
                    }
                if (search_place == "album") {
                    search_for_album (musicinformation, search_items); // associated with searching for album names 
                    search_items.clear (); // clearing out search items
            } else if (search_place == "artist") {
                search_for_artists (musicinformation, search_items); // associated with searching for album names 
                search_items.clear (); // clearing out search items
            } else if (search_place == "song") {
                search_for_songs (musicinformation, search_items);
                search_items.clear (); // clearing out search items
            }
            
            }
        }

    }while(command != "exit");    

    cout << "Thank you for using the Music Library App" << endl;
    return 0;
}
