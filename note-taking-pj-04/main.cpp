#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Image {

};


class Note {
	string _text;
	vector<Image*> _images;
public:

	Note() {}

	~Note() {}

	void setText(string text) { _text = text; }
	string getText() { return _text; }
	void addImages(Image image) { _images.push_back(&image); }
	vector<Image*> getImages() { return _images; }
};

class ILocalPersistence {
	vector<Note*> _noteList;
public:
	vector<Note*> getNotes() { return _noteList; }
	void saveNote (Note note) {};
	void updateNote (Note note) {};
	void deleteNote (Note note) {};
};

class FileManager : public ILocalPersistence {
	ifstream _fileIn;
	ofstream _fileOut;
	string _name;
	string _password;
	bool bCheck = false;
public:

	void setName(string *name) { _name = *name; }
	string getName() { return _name; }
	void setPassword(string *password) { _password = *password; }
	string getPassword() { return _password; }

	bool InPut() {
		_fileIn.open("Info.txt");
		while (_fileIn.peek() != ifstream::traits_type::eof()) {
			getline(_fileIn, _name);
			getline(_fileIn, _password);
			if (_name != "" && _password != "") {
				bCheck = true;
			}
			_fileIn.ignore();
		}
		_fileIn.close();		
		return bCheck;
	}

	void OutPut() {
		_fileOut.open("Info.txt");		
		_fileOut << _name << "\n" << _password << endl;
	}
};

class NetworkController {
	vector<Note*> _noteList;

public:
	vector<Note*> getNote() { return _noteList; }
	void create(Note Note) {};
	void updateNote(Note note) {};
	void deleteNote(Note note) {};
};


class PrivateNote : public Note {
	string _passwordHash;
public:
	void setText(string text) { _passwordHash = text; }
	string getText() { return _passwordHash; }
};



int main() {
	FileManager fManager;
	string name, password;
	if (!fManager.InPut()) {
		cout << "Enter your name" << endl;
		getline(cin, name);
		cout << "Enter Privet Notes Password" << endl;
		getline(cin, password);
		fManager.setName(&name);
		fManager.setPassword(&password);
		fManager.OutPut();
	}else
	{
		cout << "Name: " << fManager.getName() << "/n" << "Password: " << fManager.getPassword() << "/n";
	}
	return 0;
}