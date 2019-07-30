#include "MusicDB.h"


MusicDB::MusicDB(){
	//make connection and session
	
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "", err_msg, &err_code);
	if (NULL == sess) {
		cout << "error occured creating the session" << endl;
	}
	if (mysqlx_get_schema(sess, "Music_DB", 1)) {
		mysqlx_stmt_t* q2 = mysqlx_sql_new(sess, "DROP database `Music_DB`", MYSQLX_NULL_TERMINATED);
		mysqlx_execute(q2);
	}


	mysqlx_stmt_t* q = mysqlx_sql_new(sess, "create database `Music_DB`", MYSQLX_NULL_TERMINATED);
	mysqlx_execute(q);

	mysqlx_session_close(sess);
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);

	//Create tables
	/*
	1.musician
	2.album
	3.instrument
	4.producer
	5.recording_technition
	6.album_n_producer //fk
	7.musical_scene
	8.musical_scene_and_instruments //fk
	9.musical_scene_and_musician //fk
	10.musicians_and_instruments //fk
	*/
	//create table Musicians
	mysqlx_stmt_t* qTblMusicians = mysqlx_sql_new(sess,
		"CREATE TABLE `Music_DB`.`musicians` ("\
		"`musician_id` INT  NOT NULL DEFAULT 0,"\
		"`musician_name` VARCHAR(45) NOT NULL,"\
		"`musician_phone` VARCHAR(45) NOT NULL,"\
		"`musician_address` VARCHAR(45) NOT NULL,"\
		"`musician_type` INT NOT NULL,"\
		"PRIMARY KEY (`musician_id`));",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(qTblMusicians);

	//create table Album
	mysqlx_stmt_t* qTblAlbum = mysqlx_sql_new(sess,
		"CREATE TABLE `Music_DB`.`album` ("\
		"`album_id` INT  NOT NULL DEFAULT 0,"\
		"`album_name` VARCHAR(45) NOT NULL,"\
		"`record_begin` VARCHAR(45) NOT NULL,"\
		"`record_end` VARCHAR(45) NOT NULL,"\
		"PRIMARY KEY (`album_id`));",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(qTblAlbum);

	//create table instruments
	mysqlx_stmt_t* qTblInstruments = mysqlx_sql_new(sess,
		"CREATE TABLE `Music_DB`.`instrument` ("\
		"`instrument_id` INT  NOT NULL DEFAULT 0,"\
		"`instrument_type` VARCHAR(45) NOT NULL,"\
		"`manufacturer_name` VARCHAR(45) NOT NULL,"\
		"PRIMARY KEY (`instrument_id`,`manufacturer_name`,`instrument_type`));",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(qTblInstruments);

	//create table producer
	mysqlx_stmt_t* qTblProducer = mysqlx_sql_new(sess,
		"CREATE TABLE `Music_DB`.`producer` ("\
		"`producer_id` INT  NOT NULL DEFAULT 0,"\
		"`producer_name` VARCHAR(45) NOT NULL,"\
		"PRIMARY KEY (`producer_id`));",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(qTblProducer);

	//create table recording technition
	mysqlx_stmt_t* qTblTechnition = mysqlx_sql_new(sess,
		"CREATE TABLE `Music_DB`.`recording_technition` ("\
		"`technition_id` INT  NOT NULL DEFAULT 0,"\
		"`technition_name` VARCHAR(45) NOT NULL,"\
		"PRIMARY KEY (`technition_id`));",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(qTblTechnition);

	//Table with foreign keys
	mysqlx_stmt_t* qTblAlbNProducer = mysqlx_sql_new(sess,
		"CREATE TABLE `Music_DB`.`album_n_producer` ("\
		"`fk_album` int(11) NOT NULL,"\
		"`fk_producer` int(11) NOT NULL,"\
		"PRIMARY KEY (`fk_album`,`fk_producer`),"\
		"KEY `fk_producer_idx` (`fk_producer`),"\
		"CONSTRAINT `fk_album` FOREIGN KEY (`fk_album`) REFERENCES `album` (`album_id`) ON DELETE CASCADE ON UPDATE CASCADE,"\
		"CONSTRAINT `fk_producer` FOREIGN KEY (`fk_producer`) REFERENCES `producer` (`producer_id`) ON DELETE CASCADE ON UPDATE CASCADE"\
		") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci"
		, MYSQLX_NULL_TERMINATED);
	mysqlx_execute(qTblAlbNProducer);

	mysqlx_stmt_t* qMusicalScene = mysqlx_sql_new(sess,
		"CREATE TABLE `Music_DB`.`musical_scene` (\
		`fk_technition_id` INT NOT NULL,\
		`song_id` INT NOT NULL,\
		`song_name` VARCHAR(45) NOT NULL,\
		`lyrics_author_name` VARCHAR(45) NOT NULL,\
		`composure_name` VARCHAR(45) NOT NULL,\
		`length` INT NOT NULL,\
		`genere` VARCHAR(45) NOT NULL,\
		`record_date` VARCHAR(45) NOT NULL,\
		 INDEX `fk_technition_id_idx` (`fk_technition_id` ASC) VISIBLE,\
		 PRIMARY KEY(`song_id`),\
		 CONSTRAINT `fk_technition_id`\
		 FOREIGN KEY(`fk_technition_id`)\
		 REFERENCES `Music_DB`.`recording_technition` (`technition_id`)\
		 ON DELETE NO ACTION\
		 ON UPDATE NO ACTION); ",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(qMusicalScene);


	//musical Scene and musician FK
	mysqlx_stmt_t* msAndMusician = mysqlx_sql_new(sess,
		"CREATE TABLE `musical_scene_and_musician` ("\
		" `fk_song_id` int(11) NOT NULL,"\
		"`fk_musician_id` int(11) NOT NULL,"\
		"PRIMARY KEY (`fk_song_id`,`fk_musician_id`),"\
		"KEY `fk_musician_id_idx` (`fk_musician_id`),"\
		"CONSTRAINT `fk_musician_id` FOREIGN KEY (`fk_musician_id`) REFERENCES `musicians` (`musician_id`) ON DELETE CASCADE ON UPDATE CASCADE,"\
		"CONSTRAINT `fk_song_id` FOREIGN KEY (`fk_song_id`) REFERENCES `musical_scene` (`song_id`) ON DELETE CASCADE ON UPDATE CASCADE"\
		") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci"
		, MYSQLX_NULL_TERMINATED);
	mysqlx_execute(msAndMusician);


	mysqlx_stmt_t* musicianAndInst = mysqlx_sql_new(sess,
		"CREATE TABLE `musicians_and_instruments` ("\
		"`fk_musician` int(11) NOT NULL,"\
		"`fk_instrument` int(11) NOT NULL,"\
		"PRIMARY KEY (`fk_musician`,`fk_instrument`),"\
		"KEY `fk_instrument_idx` (`fk_instrument`),"\
		"CONSTRAINT `fk_instrument` FOREIGN KEY (`fk_instrument`) REFERENCES `instrument` (`instrument_id`) ON DELETE CASCADE ON UPDATE CASCADE,"\
		"CONSTRAINT `fk_musician` FOREIGN KEY (`fk_musician`) REFERENCES `musicians` (`musician_id`) ON DELETE CASCADE ON UPDATE CASCADE"\
		") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(musicianAndInst);

	mysqlx_stmt_t* albumAndMs = mysqlx_sql_new(sess,
		"CREATE TABLE `Music_DB`.`album_and_musical_scene` (\
		`album_id` INT NOT NULL,\
		`musical_scene` INT NOT NULL,\
		PRIMARY KEY(`album_id`, `musical_scene`),\
		INDEX `fk_ms_idx` (`musical_scene` ASC) VISIBLE,\
		CONSTRAINT `fk_album_id_`\
		FOREIGN KEY(`album_id`)\
		REFERENCES `Music_DB`.`album` (`album_id`)\
		ON DELETE NO ACTION\
		ON UPDATE NO ACTION,\
		CONSTRAINT `fk_ms`\
		FOREIGN KEY(`musical_scene`)\
		REFERENCES `Music_DB`.`musical_scene` (`song_id`)\
		ON DELETE NO ACTION\
		ON UPDATE NO ACTION); ",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(albumAndMs);

	mysqlx_stmt_t* musicianInstrumentMs = mysqlx_sql_new(sess,"CREATE TABLE `ms_musician_instrument` (\
		`fk_musician` int(11) NOT NULL, \
		`fk_instrument` int(11) NOT NULL, \
		`fk_ms` int(11) NOT NULL, \
		PRIMARY KEY(`fk_musician`,`fk_ms`), \
		KEY `_fk_ms_idx` (`fk_ms`), \
		KEY `_fk_instrument_idx` (`fk_instrument`), \
		CONSTRAINT `_fk_instrument` FOREIGN KEY(`fk_instrument`) REFERENCES `instrument` (`instrument_id`), \
		CONSTRAINT `_fk_ms` FOREIGN KEY(`fk_ms`) REFERENCES `musical_scene` (`song_id`), \
		CONSTRAINT `_fk_musician` FOREIGN KEY(`fk_musician`) REFERENCES `musicians` (`musician_id`)\
		) ENGINE = InnoDB DEFAULT CHARSET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(musicianInstrumentMs);

	//Insert into tables

	//insert values to musicians table
	mysqlx_stmt_t* qInsrtMusicians = mysqlx_sql_new(sess,
		"INSERT INTO musicians(musician_id,musician_name,musician_phone,musician_address,musician_type)  "\
		"VALUES  (0,'Arlene Lippman','050123120','Ana Frank 12',1),"\
				"(1,'Mia Clover','050123121','777 Brockton Avenue',2),"\
				"(2,'Joaquin Rexrode','050123122','262 Swansea Mall Dr',0),"\
				"(3,'Donette Loe','050123123','3005 Cranberry Hwy Rt 6 28',2),"\
				"(4,'Jackson Mccollum','050123124','352 Palmer Road',0),"\
				"(5,'Piper Sundstrom','050123125','297 Grant Avenu',1),"\
				"(6,'Nicolas Mariner','050123126','30 Catskill',1),"\
				"(7,'Brande Seabaugh','050123127','3018 East Ave',1),"\
				"(8,'Leana Mejias','050123128','161 Centereach Mall',0),"\
				"(9,'Mary Naff','050123129','1000 State Route 36',0),"\
				"(10,'Micheline Shoaf','050123130','100 Thruway Plaza',2);",
		//type 0 = instrument
		//type 1 = singer
		//type 2 = both
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(qInsrtMusicians);

	//insert into album
	mysqlx_stmt_t* qAlbum = mysqlx_sql_new(sess,
		"INSERT INTO album(album_id,album_name,record_begin,record_end) "\
		"VALUES  (0,'Avatar Country','2010/01/01','2011/01/01'),"\
		"(1,'Wrong Creatures','2011/01/02','2012/01/02'),"\
		"(2,'Camila','2014/01/03','2015/08/18'),"\
		"(3,'No Cross No Crown','2019/01/04','2020/11/05'),"\
		"(4,'What Happens Next','2018/01/04','2019/01/05'),"\
		"(5,'Underworld','2019/01/30','2020/12/14'),"\
		"(6,'Mania','2013/06/20','2014/07/03'),"\
		"(7,'Vertigo','2017/04/14','2018/09/05'),"\
		"(8,'Lost on the Road to Eternity','2019/5/11','2020/12/13'),"\
		"(9,'Rise to Glory','2018/07/30','2019/05/20'),"\
		"(10,'Beloved Antichrist','2011/02/27','2013/04/12');",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(qAlbum);


	mysqlx_stmt_t* insrtInstrument = mysqlx_sql_new(sess,
	"INSERT INTO instrument (instrument_id, instrument_type, manufacturer_name) VALUES (0,'Guitar','Cley Zemer'),\
 (1,'Guitar','BassLab'),\
 (2,'Guitar','Yamaha'),\
 (3,'Piano','BassLab'),\
 (4,'Piano','Yamaha'),\
 (5,'Piano','intruments INC'),\
 (6,'Clarient','BassLab'),\
 (7,'Clarient','Cley Zemer'),\
 (8,'Bass Guitar','BassLab'),\
 (9,'Cello','BassLab'),\
 (10,'Violin','BassLab');",
		MYSQLX_NULL_TERMINATED);
	mysqlx_result_t* res =  mysqlx_execute(insrtInstrument);
	if (res == NULL)
		cout << "Error";

	mysqlx_stmt_t* qProducer = mysqlx_sql_new(sess,
		"INSERT INTO producer(producer_id,producer_name)\
		VALUES  (0,'Kenneth Hager '),\
		(1,'Lino Herder'),\
		(2,'Nanci Lichtenberger '),\
		(3,'Aimee Pendergrass '),\
		(4,'Rosy Hahne'),\
		(5,'Haywood Lovett'),\
		(6,'Coleman Greenly '),\
		(7,'Jerrod Nack'),\
		(8,'Larae Cossette'),\
		(9,'Olga Behling'),\
		(10,'Preston Matta');",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(qProducer);

	mysqlx_stmt_t* qTechnition = mysqlx_sql_new(sess,
		"INSERT INTO recording_technition(technition_id,technition_name)\
		VALUES  (0,'Son Shipman'),\
		(1,'Anderson Brouwer'),\
		(2,'Waldo Mccraney'),\
		(3,'Major Yelle'),\
		(4,'Wendell Kai'),\
		(5,'Stanford Butera'),\
		(6,'Trey Taitt'),\
		(7,'Cory Perez'),\
		(8,'Leonard Shanklin'),\
		(9,'Jon Mitts'),\
		(10,'Harris Tempel');",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(qTechnition);

		mysqlx_stmt_t* albAndproducer = mysqlx_sql_new(sess,
		"INSERT INTO album_n_producer(fk_album, fk_producer)\
		VALUES  (0,1),\
		(1,1),\
		(1,2),\
		(2,9),\
		(3,3),\
		(4,3),\
		(5,4),\
		(6,1),\
		(7,9),\
		(8,7),\
		(9,5),\
		(10,7);",
			MYSQLX_NULL_TERMINATED);
	mysqlx_execute(albAndproducer);

	mysqlx_stmt_t* msInsrt = mysqlx_sql_new(sess,
		"INSERT INTO musical_scene (song_id, song_name, lyrics_author_name,composure_name, length, genere, record_date, fk_technition_id)\
		VALUES(1, 'Californication', 'Avi Gabay', 'Arik Nesher', 200, 'Rock', '2005/02/01', 1),\
		(2, 'Zephir song', 'Anthony Kiedis', 'Rick Rubin', 210, 'Pop', '2012/08/13', 0),\
		(3, 'Aeroplane', 'Anthony Kiedis', 'Rick Rubin', 200, 'Rock', '2011/07/27', 1),\
		(4, 'Annie Wants a Baby', 'Anthony Kiedis', 'Rick Rubin', 180, 'Rock', '2013/06/28', 2),\
		(5, 'By The Way', 'Flea', 'George Clinton', 185, 'Rock', '2011/10/07', 3),\
		(6, 'Heaven', 'Flea', 'George Clinton', 222, 'Jazz', '2019/05/15', 4),\
		(7, 'Yellow', 'Cliff Martinez', 'Andy Gill', 225, 'Jazz', '2018/09/21', 5),\
		(8, 'Yellow light', 'Cliff Martinez', 'Andy Gill', 212, 'Jazz', '2012/02/16', 6),\
		(9, 'Cant stop', 'John Frusciante', 'Andy Gill', 192, 'Pop', '2011/01/07', 9),\
		(10, 'Dark Necessities', 'John Frusciante', 'Andy Gill', 195, 'Rock', '2011/12/15', 10); ",
		MYSQLX_NULL_TERMINATED);
	res = mysqlx_execute(msInsrt);

	//musical_scene_and_musician
	mysqlx_stmt_t* msAndMusician2 = mysqlx_sql_new(sess,
		"INSERT INTO musical_scene_and_musician(fk_song_id, fk_musician_id)\
		VALUES (1,2),\
		(1,5),\
		(1,6),\
		(2,9),\
		(3,6),\
		(4,7),\
		(4,2),\
		(5,8),\
		(6,1),\
		(6,0),\
		(7,8),\
		(7,10),\
		(8,2),\
		(8,3),\
		(8,7),\
		(8,9),\
		(9,0),\
		(10,5),\
		(10,7);",
		//make sure that i have a producer!
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(msAndMusician2);

	//musical_scene_n_instrument
	mysqlx_stmt_t* msAndInsrtument = mysqlx_sql_new(sess,
		"INSERT INTO musical_scene_n_instrument(fk_song_id, fk_instrument_id)\
		VALUES (1,0),\
		(1,2),\
		(1,6),\
		(2,9),\
		(3,6),\
		(4,7),\
		(4,2),\
		(5,8),\
		(6,1),\
		(6,0),\
		(7,8),\
		(7,10),\
		(8,2),\
		(8,3),\
		(8,7),\
		(8,9),\
		(9,0),\
		(10,5),\
		(10,7);",
		//make sure that i have a producer!
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(msAndInsrtument);

	//musicians_and_instruments
	//musician plays on this instruments
	mysqlx_stmt_t* musicianAndInstrument = mysqlx_sql_new(sess,
		"INSERT INTO musicians_and_instruments(fk_musician, fk_instrument)\
		VALUES ( 1,2),\
		(1,5),\
		(1,8),\
		(2,1),\
		(3,9),\
		(4,2),\
		(4,8),\
		(8,1),\
		(8,2),\
		(8,8),\
		(9,7),\
		(10,1),\
		(10,2);",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(musicianAndInstrument);

	//album_and_musical_scene
	mysqlx_stmt_t* insrtAlbumAndMs = mysqlx_sql_new(sess,
		"INSERT INTO album_and_musical_scene(album_id, musical_scene)\
		VALUES (0,1),\
		(1,1),\
		(2,1),\
		(2,9),\
		(2,10),\
		(3,3),\
		(3,6),\
		(4,5),\
		(5,5),\
		(6,4),\
		(7,2),\
		(8,5),\
		(9,7),\
		(10,8);",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(insrtAlbumAndMs);
	//insert into ms_musician_instrument
	mysqlx_stmt_t* insrtMsMusicInstrument = mysqlx_sql_new(sess,
		"INSERT INTO ms_musician_instrument(fk_musician, fk_instrument,fk_ms)\
		VALUES(1, 2, 6),\
		(2, 1, 1),\
		(2, 1, 4),\
		(2, 1, 8),\
		(3, 9, 8),\
		(8, 1, 5),\
		(8, 8, 7),\
		(9, 7, 2),\
		(9, 7, 8),\
		(10, 1, 7);",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(insrtMsMusicInstrument);

	mysqlx_stmt_t* msMusicianNnstrument = mysqlx_sql_new(sess,
		"",
		MYSQLX_NULL_TERMINATED);
	mysqlx_execute(msMusicianNnstrument);

	cout << "Data base " << "'DB name '" << "has been initialized" << endl;
	mysqlx_session_close(sess);
}


MusicDB::~MusicDB(){

}

//action 1
void MusicDB::albumsRecorded() {
	string d1, d2;
	cout << "action 1:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	cout << "Enter date(y1):";
	cin >> d1;
	cout << "Enter date(y2):";
	cin >> d2;
	string query = "SELECT count(*) as albums_sum FROM album where record_end between '"+d1+"' AND '"+d2+"';";
	mysqlx_stmt_t* q1 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q1);

	if (NULL != res) {
		char buff[256]; 
		size_t size = 0;
		mysqlx_row_t* row = mysqlx_row_fetch_one(res);
		int64_t id;
		if (row != NULL) {
			mysqlx_get_sint(row, 0, &id);
			cout << "Albums recorded between '" + d1 + "' and '" + d2 +"' : ";
			cout << id << endl;
		}
	}
	mysqlx_session_close(sess);
}

//action 2
void MusicDB::diffSongsByMusician() {
	string d1, d2, musicianName;
	cout << "action 2:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	cout << "Enter musician name: ";
	getline(cin, musicianName);	
	getline(cin, musicianName);
	cout << "Enter date(y1):";
	cin >> d1;
	cout << "Enter date(y2):";
	cin >> d2;
	string query = "SELECT COUNT(*) AS song_count\
 FROM(SELECT record_date\
 FROM musical_scene\
 WHERE song_id IN(SELECT fk_song_id\
 FROM musical_scene_and_musician\
 WHERE fk_musician_id = (SELECT musician_id\
 FROM musicians\
 WHERE  musician_name = '"+ musicianName +"'))) S\
 WHERE S.record_date between "+ d1 +" AND "+ d2+";";
	mysqlx_stmt_t* q2 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q2);
	if (NULL != res) {
		char buff[256];
		size_t size = 0;
		mysqlx_row_t* row = mysqlx_row_fetch_one(res);
		int64_t id;
		if (row != NULL) {
			mysqlx_get_sint(row, 0, &id);
			cout << "Albums recorded between '" + d1 + "' and '" + d2 + "' : ";
			cout << id << endl;
		}
	}
	else cout << musicianName << " hasn't recorded any song" << endl;
	mysqlx_session_close(sess);
}

//action 3
void MusicDB::musiciainDiffAlbums() {
	string d1, d2, musicianName;
	cout << "action 3:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	cout << "Enter musician name: ";
	getline(cin, musicianName);
	getline(cin, musicianName);
	cout << "Enter date(y1):";
	cin >> d1;
	cout << "Enter date(y2):";
	cin >> d2;
	string query = "SELECT COUNT(*) AS albums_count\
 FROM(SELECT record_begin\
 FROM album\
 WHERE album_id IN(SELECT album_id\
 FROM album_and_musical_scene\
 WHERE musical_scene IN(SELECT fk_song_id\
 FROM musical_scene_and_musician\
 WHERE fk_musician_id = (SELECT musician_id\
 FROM musicians\
 WHERE  musician_name = '"+ musicianName +"')))) S\
 WHERE record_begin BETWEEN '"+d1+"' AND '"+d2+"';";
	mysqlx_stmt_t* q3 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q3);
	if (NULL != res) {
		char buff[256];
		size_t size = 0;
		mysqlx_row_t* row = mysqlx_row_fetch_one(res);
		int64_t id;
		if (row != NULL) {
			mysqlx_get_sint(row, 0, &id);
			
			cout << "Diiferent albums where " << musicianName << " participated: " << id << endl;
		}
	}
	mysqlx_session_close(sess);
}

//action 4
void MusicDB::commonInstrument() {
	cout << "action 4:" << endl;
	string get;
	cin >> get;
	//i made it only because the compiler doesnt stop on breakpoint unless i get something from the screen
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	string query = "SELECT instrument_type as popular_instrument FROM (SELECT fk_musician, instrument_type\
 FROM instrument AS i JOIN musicians_and_instruments as mi ON mi.fk_instrument = i.instrument_id\
 WHERE fk_musician IN (SELECT fk_musician_id FROM musical_scene AS ms\
 JOIN musical_scene_and_musician as msi ON msi.fk_song_id = ms.song_id)) it\
 GROUP by instrument_type ORDER BY COUNT(*) DESC LIMIT 1;";

	mysqlx_stmt_t* q4 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q4);

	if (NULL != res) {
		char buff[256];
		size_t size = 0;
		mysqlx_row_t* row = mysqlx_row_fetch_one(res);
		int64_t i = 0;
		size = sizeof(buff);
		mysqlx_get_bytes(row, 0, 0, buff, &size);
		cout << "The most popular instrument is: " << buff << endl;
	}
	mysqlx_session_close(sess);
}

//action 5
void MusicDB::showInstruments() {
	getchar();
	cout << "action 5:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	string album;
	cout << "Enter Album name:";
	getline(cin, album);

	string query = "SELECT instrument_type\
 FROM instrument as i\
 JOIN musicians_and_instruments AS mi ON mi.fk_instrument = i.instrument_id\
 JOIN musical_scene_and_musician as msm ON msm.fk_musician_id = mi.fk_musician\
 JOIN album_and_musical_scene AS ams ON ams.musical_scene = msm.fk_song_id\
 JOIN album as a ON a.album_id = ams.album_id\
 WHERE a.album_name = '"+album+"';";
	cout << query << endl;
	mysqlx_stmt_t* q5 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q5);

	if (NULL != res) {
		char buff[256]; size_t size = 0;
		cout << "The instrument that appered in album " << album << " are:" << endl;
		do {
			mysqlx_row_t* row = mysqlx_row_fetch_one(res);

			while (NULL != row) {
				size = sizeof(buff);
				mysqlx_get_bytes(row, 0, 0, buff, &size);
				cout << buff;
				row = mysqlx_row_fetch_one(res);
				cout << endl;
			}
		} while (RESULT_OK == mysqlx_next_result(res));
	}
	else {
		cerr << mysqlx_error_message(sess) << endl;
		exit(mysqlx_error_num(sess));
	}
	mysqlx_session_close(sess);
}

//action 6
void MusicDB::fertileProducer() {
	getchar();
	cout << "action 6:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	string query = "SELECT producer_name, COUNT(producer_id) AS most_productive_pro\
 FROM producer AS p\
 JOIN album_n_producer AS ap ON p.producer_id = ap.fk_producer\
 GROUP BY producer_id\
 order by most_productive_pro DESC LIMIT 1; ";

	mysqlx_stmt_t* q6 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q6);
	if (NULL != res) {
		char buff[256]; size_t size = 0;
		cout << "The most productive producer is: ";
		mysqlx_row_t* row = mysqlx_row_fetch_one(res);

		while (NULL != row) {
			size = sizeof(buff);
			mysqlx_get_bytes(row, 0, 0, buff, &size);
			cout << buff;
			row = mysqlx_row_fetch_one(res);
			cout << endl;
		}
	}
	else {
		cerr << mysqlx_error_message(sess) << endl;
		exit(mysqlx_error_num(sess));
	}

	mysqlx_session_close(sess);
}

//action 7
void MusicDB::popularManufacturer() {
	getchar();
	cout << "action 7:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);

	string query = "SELECT manufacturer_name\
 FROM instrument\
 WHERE instrument_id = (SELECT fk_instrument\
 FROM(SELECT fk_instrument, COUNT(*)\
 FROM(SELECT fk_instrument\
 FROM ms_musician_instrument\
 WHERE fk_instrument IN(SELECT fk_instrument\
 FROM ms_musician_instrument)\
 GROUP BY fk_instrument)fk_ins)ins_cnt); ";

	mysqlx_stmt_t* q7 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q7);
	if (NULL != res) {
		char buff[256]; size_t size = 0;
		cout << "The most popular manufacturer is: ";
		mysqlx_row_t* row = mysqlx_row_fetch_one(res);

		if (NULL != row) {
			size = sizeof(buff);
			mysqlx_get_bytes(row, 0, 0, buff, &size);
			cout << buff << endl;
		}
	}
	else {
		cerr << mysqlx_error_message(sess) << endl;
		exit(mysqlx_error_num(sess));
	}

	mysqlx_session_close(sess);
}

//action 8
void MusicDB::musicianRecording() {
	getchar();
	cout << "action 8:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	string query = "SELECT COUNT(DISTINCT fk_musician_id) AS recording_for_all_years\
 FROM musical_scene_and_musician";

	mysqlx_stmt_t* q8 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q8);
	if (NULL != res) {
		char buff[256];
		size_t size = 0;
		mysqlx_row_t* row = mysqlx_row_fetch_one(res);
		int64_t id;
		if (row != NULL) {
			mysqlx_get_sint(row, 0, &id);
			cout << "Number of musicians the recorded over the years: " << id << endl;
		}
	}
	mysqlx_session_close(sess);
}

//action 9
void MusicDB::musicianWithMusicians() {
	getchar();
	cout << "action 9:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	string query = "SELECT musician_name FROM musicians\
 WHERE musician_id = (SELECT fk_musician_id FROM(SELECT fk_musician_id\
 FROM musical_scene_and_musician\
 WHERE fk_song_id IN(SELECT fk_song_id\
 FROM musical_scene_and_musician\
 GROUP BY fk_song_id HAVING COUNT(*)>1\
 ORDER BY COUNT(*) DESC))AS mi\
 GROUP BY mi.fk_musician_id\
 ORDER BY COUNT(*) DESC\
 LIMIT 1); ";

	mysqlx_stmt_t* q9 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q9);
	if (NULL != res) {
		char buff[256]; size_t size = 0;
		cout << "\nA musician who collaborated with the greatest number of other musicians: ";
		mysqlx_row_t* row = mysqlx_row_fetch_one(res);

		while (NULL != row) {
			size = sizeof(buff);
			mysqlx_get_bytes(row, 0, 0, buff, &size);
			cout << buff << endl;
			row = mysqlx_row_fetch_one(res);
		}
	}
	else {
		cerr << mysqlx_error_message(sess) << endl;
		exit(mysqlx_error_num(sess));
	}
	mysqlx_session_close(sess);
}

//action 10
void MusicDB::popularGenere() {
	getchar();
	cout << "action 10:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	string query = "SELECT genere,COUNT(*) AS most_pop_genere\
 FROM musical_scene\
 GROUP BY genere\
 ORDER BY most_pop_genere DESC LIMIT 1; ";

	mysqlx_stmt_t* q10 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q10);
	if (NULL != res) {
		char buff[256]; size_t size = 0;
		cout << "\nMost popular genere is: ";
		mysqlx_row_t* row = mysqlx_row_fetch_one(res);
		if (res != NULL) {
			size = sizeof(buff);
			mysqlx_get_bytes(row, 0, 0, buff, &size);
			cout << buff << endl;
		}
	}
	else {
		cerr << mysqlx_error_message(sess) << endl;
		exit(mysqlx_error_num(sess));
	}
	mysqlx_session_close(sess);
}



//action 11
void MusicDB::technitionMostSongs() {
	getchar();
	cout << "action 11:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	string query = "SELECT technition_name\
 FROM recording_technition\
 WHERE technition_id = (SELECT fk_technition_id\
 FROM musical_scene\
 GROUP BY fk_technition_id HAVING COUNT(*)>1); ";

	mysqlx_stmt_t* q11 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q11);
	if (NULL != res) {
		char buff[256]; size_t size = 0;
		cout << "\nMost popular genere is: ";
		mysqlx_row_t* row = mysqlx_row_fetch_one(res);
		if (res != NULL) {
			size = sizeof(buff);
			mysqlx_get_bytes(row, 0, 0, buff, &size);
			cout << buff << endl;
		}
	}
	else {
		cerr << mysqlx_error_message(sess) << endl;
		exit(mysqlx_error_num(sess));
	}
	mysqlx_session_close(sess);
}

//action 12
void MusicDB::firstAlbumRecorded() {
	getchar();
	cout << "action 12:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	string query = "SELECT album_name, min(record_end)\
 FROM album";

	mysqlx_stmt_t* q12 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q12);
	if (NULL != res) {
		char buff[256]; size_t size = 0;
		cout << "\nFirst album recorded is: ";
		mysqlx_row_t* row = mysqlx_row_fetch_one(res);
		if (res != NULL) {
			size = sizeof(buff);
			mysqlx_get_bytes(row, 0, 0, buff, &size);
			cout << buff << endl;
		}
	}
	else {
		cerr << mysqlx_error_message(sess) << endl;
		exit(mysqlx_error_num(sess));
	}

	mysqlx_session_close(sess);
}

//action 13
void MusicDB::songsFromMoreThanOneAlbum() {
	getchar();
	cout << "action 13:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	string query = "SELECT song_name\
 FROM musical_scene\
 WHERE song_id IN(SELECT musical_scene\
 FROM album_and_musical_scene\
 GROUP BY musical_scene HAVING COUNT(*)>1); ";

	mysqlx_stmt_t* q13 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q13);

	if (NULL != res) {
		char buff[256]; size_t size = 0;
		cout << "Musical scenes that appeared in more than 1 album are:  " << endl;
		do {
			mysqlx_row_t* row = mysqlx_row_fetch_one(res);

			while (NULL != row) {
				size = sizeof(buff);
				mysqlx_get_bytes(row, 0, 0, buff, &size);
				cout << buff;
				row = mysqlx_row_fetch_one(res);
				cout << endl;
			}
		} while (RESULT_OK == mysqlx_next_result(res));
	}
	else {
		cerr << mysqlx_error_message(sess) << endl;
		exit(mysqlx_error_num(sess));
	}
	mysqlx_session_close(sess);
}

//action 14
void MusicDB::tecthnitionOfFullAlbum() {
	getchar();
	cout << "action 14:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	string query = "SELECT technition_name\
 FROM recording_technition\
 WHERE technition_id IN(SELECT DISTINCT musical_scene.fk_technition_id\
 FROM musical_scene\
 WHERE musical_scene.song_id IN(SELECT DISTINCT musical_scene\
 FROM album_and_musical_scene INNER JOIN(SELECT album_id\
 FROM(SELECT album_id, COUNT(*)\
 FROM(SELECT DISTINCT fk_technition_id, album_id\
 FROM musical_scene\
 JOIN album_and_musical_scene ON album_and_musical_scene.musical_scene = musical_scene.song_id)a\
 GROUP BY album_id\
 HAVING COUNT(*) = 1)alb)ms ON album_and_musical_scene.album_id = ms.album_id)); ";

	mysqlx_stmt_t* q14 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q14);

	if (NULL != res) {
		char buff[256]; size_t size = 0;
		cout << "Technitions who recorded solely full album are:  " << endl;
		do {
			mysqlx_row_t* row = mysqlx_row_fetch_one(res);

			while (NULL != row) {
				size = sizeof(buff);
				mysqlx_get_bytes(row, 0, 0, buff, &size);
				cout << buff;
				row = mysqlx_row_fetch_one(res);
				cout << endl;
			}
		} while (RESULT_OK == mysqlx_next_result(res));
	}
	else {
		cerr << mysqlx_error_message(sess) << endl;
		exit(mysqlx_error_num(sess));
	}
	mysqlx_session_close(sess);
}

//action 15
void MusicDB::mostDiversedMusician() {
	getchar();
	cout << "action 15:" << endl;
	sess = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, "root", "gERshman8", "Music_DB", err_msg, &err_code);
	string query = "SELECT musician_name\
 FROM(SELECT distinct musician_id, musician_name, genere\
 FROM musicians AS mn\
 JOIN musical_scene_and_musician AS msm ON msm.fk_musician_id = mn.musician_id\
 JOIN musical_scene AS ms ON ms.song_id = msm.fk_song_id) a\
 GROUP BY musician_name having count(*)>1; ";

	mysqlx_stmt_t* q15 = mysqlx_sql_new(sess, query.c_str(), MYSQLX_NULL_TERMINATED);
	mysqlx_result_t*  res = mysqlx_execute(q15);

	if (NULL != res) {
		char buff[256]; size_t size = 0;
		cout << "The most versatile musician are:  " << endl;
		do {
			mysqlx_row_t* row = mysqlx_row_fetch_one(res);

			while (NULL != row) {
				size = sizeof(buff);
				mysqlx_get_bytes(row, 0, 0, buff, &size);
				cout << buff;
				row = mysqlx_row_fetch_one(res);
				cout << endl;
			}
		} while (RESULT_OK == mysqlx_next_result(res));
	}
	else {
		cerr << mysqlx_error_message(sess) << endl;
		exit(mysqlx_error_num(sess));
	}

	mysqlx_session_close(sess);
}

