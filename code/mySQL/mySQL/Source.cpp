#include <iostream>
#include <cstring>
#include <cstdlib>
#include "MusicDB.h"
#include "C:\Program Files\MySQL\Connector C++ 8.0\include\mysqlx\xapi.h"

using namespace std;

int main(int argc, char *argv[]) {
	int action;
	bool out = 1;
	MusicDB db;//Data Base initialization
	while (out) {
		cout << "Please choose an action from to the following list:\n" << endl;
		cout << "Press ans other key to exit" << endl;
		cout << "1.How many album recorded between Y1 until Y2 date" << endl;
		cout << "2.How many different songs musician X recorded between Y1 until Y2 date" << endl;
		cout << "3.In how many different albums muaisican X participated between Y1 until Y2 date" << endl;
		cout << "4.The most popular instrument throughout the years" << endl;
		cout << "5.List of all instruments in album A" << endl;
		cout << "6.The most productive producer between date Y1 until Y2" << endl;
		cout << "7.The most popular manufacturer" << endl;
		cout << "8.How many musicians recorded throughout the years" << endl;
		cout << "9.The musician who cooperated with the greatest number of other musicians " << endl;
		cout << "10.The most popular song genre (throughout the years)" << endl;
		cout << "11.Who is the recording technician who recorded the largest number of songs between Y1 and Y2 date" << endl;
		cout << "12.What is the first album recorded in the studio" << endl;
		cout << "13.List all songs in 2 or more albums" << endl;
		cout << "14.List of technicians who have shared the recording of an entire album" << endl;
		cout << "15.Who is the most versatile musician (the musician who recorded songs In the widest variety of genres)" << endl;
		cin >> action;
		switch (action) {
		case 1:
			db.albumsRecorded();
			break;
		case 2:
			db.diffSongsByMusician();
			break;
		case 3:
			db.musiciainDiffAlbums();
			break;
		case 4:
			db.commonInstrument();
			break;
		case 5:
			db.showInstruments();
			break;
		case 6:
			db.fertileProducer();
			break;
		case 7:
			db.popularManufacturer();
			break;
		case 8:
			db.musicianRecording();
			break;
		case 9:
			db.musicianWithMusicians();
			break;
		case 10:
			db.popularGenere();
			break;
		case 11:
			db.technitionMostSongs();
			break;
		case 12:
			db.firstAlbumRecorded();
			break;
		case 13:
			db.songsFromMoreThanOneAlbum();
			break;
		case 14:
			db.tecthnitionOfFullAlbum();
			break;
		case 15:
			db.mostDiversedMusician();
			break;
		default:
			out = 0;
			break;
		}
	}
	return 0;
}