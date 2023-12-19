
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <boost/filesystem.hpp>
#include <urlmon.h>
#include <typeinfo>
#include <locale>

#pragma comment (lib, "urlmon.lib") // показываем компоновщику что будем использовать эту внешнюю библиотеку 

namespace fs = boost::filesystem;
using namespace std;

HRESULT DownloadFile(string line, fs::path full_name, string chat_name, int& k) {
    int start = line.find("https://sun");
    int end = line.find("=album") + 6; //line.find_last_of("=album")+1

    string link = line.substr(start, end - start);
    wstring w_link = wstring(link.begin(), link.end());

    wstring temp = full_name.wstring();
    string photo_full_name;
    wstring w_photo_full_name;
    cout << full_name << endl;

    if (temp.find(L"bookmarks") != std::string::npos)
    {
        photo_full_name = "C:\\Users\\Alex\\Downloads\\Archive\\ALL_PHOTO_AND_VIDEO\\BOOKMARKS\\"; // СДЕЛАТЬ ПУТИ ОТ ТОТГО ЧТО ВВЁЛ ПОЛЬЗОАВТЕЛЬ. ВЕДЬ они разные
    }
    if (temp.find(L"likes") != std::string::npos)
    {
        photo_full_name = "C:\\Users\\Alex\\Downloads\\Archive\\ALL_PHOTO_AND_VIDEO\\LIKES\\";
    }
    if (temp.find(L"messages") != std::string::npos)
    {
        photo_full_name = "C:\\Users\\Alex\\Downloads\\Archive\\ALL_PHOTO_AND_VIDEO\\MESSAGES\\";
    }
    if (temp.find(L"photos") != std::string::npos)
    {
        photo_full_name = "C:\\Users\\Alex\\Downloads\\Archive\\ALL_PHOTO_AND_VIDEO\\PHOTOS\\";
    }
    if (temp.find(L"profile") != std::string::npos)
    {
        photo_full_name = "C:\\Users\\Alex\\Downloads\\Archive\\ALL_PHOTO_AND_VIDEO\\PROFILE\\";
    }
    if (temp.find(L"video") != std::string::npos)
    {
        photo_full_name = "C:\\Users\\Alex\\Downloads\\Archive\\ALL_PHOTO_AND_VIDEO\\VIDEO\\";
    }
    if (temp.find(L"wall") != std::string::npos)
    {
        photo_full_name = "C:\\Users\\Alex\\Downloads\\Archive\\ALL_PHOTO_AND_VIDEO\\WALL\\";
    }

    photo_full_name = photo_full_name + chat_name + " (" + to_string(k) + ")" + ".jpg";
    HRESULT download_start = URLDownloadToFileA(0, link.c_str(), photo_full_name.c_str(), 0, 0);


    //photo_full_name = photo_full_name + "ЫЫЫЫЫЫЫЫ" + '(' + to_string(k) + ')' + ".jpg";
    //
    //w_photo_full_name = wstring(photo_full_name.begin(), photo_full_name.end());
    //cout << w_photo_full_name.c_str() << "<-----";
    //cout << photo_full_name.c_str();

    //HRESULT download_start = URLDownloadToFile(0, (LPCWSTR)w_link.c_str(), (LPCWSTR)w_photo_full_name.c_str(), 0, 0); //class="ui_crumb" >ИМЯ С КЕМ ПЕРЕПИСКА<
    return download_start;
}

bool ReadFileHtml(fs::path full_name, int& k)
{
    string line;
    string chat_name = " "; //default name
    ifstream in(full_name.string());
    int s;

    if (in.is_open())
    {
        while (getline(in, line))
        {
            if (line.find("<div class=\"ui_crumb\" >") != std::string::npos) // название чата всегда раньше самой переписки и фоток
            {

                int start_name = line.rfind("\" >") + 3; // начало имени
                int end_name = line.find("</div>", start_name - 1); // конец имени. Ищем div начиная с начала имени.

                cout << "START: " << start_name;
                cout << "END: " << end_name;

                chat_name = line.substr(start_name, end_name - start_name);

                cout << " " << chat_name << endl;
            }

            if (line.find("jpg") != std::string::npos)
            {
                if (line.find("https://sun") != std::string::npos)
                {
                    if (DownloadFile(line, full_name, chat_name, k) == S_OK)
                    {
                        cout << "Photo number " << k++ << " is being downloaded\n";
                    }
                }
            }
        }

    }

    in.close();
    return 0;
}


bool CreateDirectories(fs::path folder)
{
    folder += "\\ALL_PHOTO_AND_VIDEO";
    fs::path original_path = folder;

    if (!fs::create_directory(folder)) { return 0; }

    if (!fs::create_directory(folder += "\\BOOKMARKS")) { return 0; }

    folder = original_path;
    if (!fs::create_directory(folder += "\\LIKES")) { return 0; }

    folder = original_path;
    if (!fs::create_directory(folder += "\\MESSAGES")) { return 0; }

    folder = original_path;
    if (!fs::create_directory(folder += "\\PHOTOS")) { return 0; }

    folder = original_path;
    if (!fs::create_directory(folder += "\\PROFILE")) { return 0; }

    folder = original_path;
    if (!fs::create_directory(folder += "\\VIDEO")) { return 0; }

    folder = original_path;
    if (!fs::create_directory(folder += "\\WALL")) { return 0; }

    return 1;
}
int main()
{
    std::locale system("");
    std::locale::global(system);
    //system("chcp 1251");
    setlocale(LC_ALL, "RUS");
    int k = 0;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // получаем хэндл(указатель) на STD вывод (cout)

    fs::path p("C:\\Users\\Alex\\Downloads\\Archive"); // стандартный путь к папке

    cout << "Enter the full path to the folder. "
        "Example: \"C:\\Users\\Alex\\Downloads\\Archive\" \n";

    //cin >> p; // ввод пути
    CreateDirectories(p);

    fs::path full_name;   // полный путь (строка) к файлу
    fs::path file_name;   // путь (строка) к файлу

    for (fs::recursive_directory_iterator i(p), end; i != end; i++) // проход по всем файлам и папкам
    {
        if (!is_directory(i->path())) // если это файл
        {
            full_name = i->path();
            file_name = i->path().filename();

            //SetConsoleTextAttribute(hConsole, 10);
            //cout << setiosflags(ios::left) << setw(80)<<full_name; // вывод полного пути файла
            //
            //SetConsoleTextAttribute(hConsole, 14);
            //cout << "------->\t";
            //
            //SetConsoleTextAttribute(hConsole, 11); 
            //cout<<file_name << endl; // вывод имени файла

            //SetConsoleTextAttribute(hConsole, 15); // стандартный цвет

            if (fs::path(file_name).extension() == ".html")
            {
                ReadFileHtml(full_name, k);
            }





        }
    }
    cout << "Total photos found: " << k;
}
