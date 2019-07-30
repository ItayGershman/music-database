#include <string>
#include <iostream>
#include <mysqlx/xapi.h>
#define DB_NAME "db_project_music"

using namespace std;

class MusicDB {
private:
	mysqlx_session_t* sess;
	char err_msg[256];
	int err_code;
public:

	//15 actions
	void albumsRecorded();
	void diffSongsByMusician();
	void musiciainDiffAlbums();
	void commonInstrument();
	void showInstruments();
	void fertileProducer();
	void popularManufacturer();
	void musicianRecording();
	void musicianWithMusicians();
	void popularGenere();
	void technitionMostSongs();
	void firstAlbumRecorded();
	void songsFromMoreThanOneAlbum();
	void tecthnitionOfFullAlbum();
	void mostDiversedMusician();

	MusicDB();
	~MusicDB();
};

