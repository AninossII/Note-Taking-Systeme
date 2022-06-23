#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ILocalPersistence {
	vector<Note*> _noteList;
public:
	vector<Note*> getNotes() { return _noteList; }
	void saveNote (Note note) {};
	void updateNote (Note note) {};
	void deleteNote (Note note) {};
};

class FileManager : public ILocalPersistence {

};

class NetworkController {
	vector<Note*> _noteList;

public:
	vector<Note*> getNote() { return _noteList; }
	void create(Note Note) {};
	void updateNote(Note note) {};
	void deleteNote(Note note) {};
};

class Image {

};

class Note {
	string _text;
	vector<Image*> _images;
public:

	Note(){}

	~Note(){}

	void setText(string text) { _text = text; }
	string getText() { return _text; }
	void addImages(Image image) { _images.push_back(&image); }
	vector<Image*> getImages() { return _images; }
};

class PrivateNote : public Note {
	string _passwordHash;
public:
	void setText(string text) { _passwordHash = text; }
	string getText() { return _passwordHash; }
};

class Crypto {
public:
	string hash(string input) {
		return input;
	}
};

int main() {

	return 0;
}