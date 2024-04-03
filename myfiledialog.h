#ifndef MYFILEDIALOG_H
#define MYFILEDIALOG_H

#include <QDialog>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QFileIconProvider>
#include <QImageReader>
#include <QImageWriter>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QLabel>
#include <QIcon>
#include <QComboBox>
#include <QStandardPaths>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QScroller>
#include <QScrollBar>
#include <QScreen>
#include <QFileInfo>
#include <QFile>
#include <QWindow>

#define Icon_Prepared

#ifndef API_H
#ifdef Q_OS_WIN
//Windows
#define Q_Device_Desktop
#else
#ifdef Q_OS_LINUX
#ifdef Q_OS_ANDROID
//Android
#define Q_Device_Mobile
#else
//Linix Desktop
#define Q_Device_Desktop
#endif
#else
#ifdef Q_OS_UNIX
#ifdef Q_OS_MAC
//Macos
#define Q_Device_Desktop
#else
#ifdef Q_OS_IOS
//ios
#define Q_Device_Mobile
#else
//unix unknown
#define Q_Device_Unknown
#endif
#endif
#else
//Unknown
#define Q_Device_Unknown
#endif
#endif
#endif

#ifdef Q_Device_Unknown
#define Q_Device_Desktop
#endif
#endif

static QString getlastPath_private(QString path)
{
    QString ret = path;

    if(ret.indexOf("/") != ret.lastIndexOf("/"))
    {
        if(ret.endsWith("/"))
            ret.chop(1);

        while(!ret.endsWith("/"))
        {
            ret.chop(1);
        }
    }
#ifdef Q_OS_WIN
    if(ret == path && ret != "我的电脑/")
    {
        ret = "我的电脑";
    }
#endif
    return ret;
}
static QFileInfoList getFileList_priavte(QString path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folder_list.size(); i++)
    {
         QString name = folder_list.at(i).absoluteFilePath();
         QFileInfoList child_file_list = getFileList_priavte(name);
         file_list.append(child_file_list);
    }

    return file_list;
}
static bool hasFilePermission_private(QString filepath,QIODevice::OpenModeFlag device = QIODevice::ReadWrite)//检查文件访问权限
{
    bool check = true;
    if(filepath != "")
    {
        QFile* file = new QFile(filepath);
        if(!file->exists())
            check = false;
        if(file->open(device))
        {
            file->close();
            delete file;
            if(!check)
                QFile::remove(filepath);
            return true;
        }
        else
        {
            delete file;
            return false;
        }
    }
    else
    {
        return false;
    }
}
static QStringList toStringList_private(QList<QByteArray> list)
{
    QStringList ret;
    for(QByteArray byte : list)
    {
        ret << QString(byte);
    }
    return ret;
}
static QByteArray readfile_private(QString path)
{
    if(path != "")
    {
        QFile* file = new QFile(path);
        if(file->open(QIODevice::ReadOnly) || file->open(QIODevice::ReadWrite))
        {
            QByteArray content = file->readAll();
            file->close();
            delete file;
            return content;
        }
        else
        {
            return QByteArray();
        }
    }
    else
    {
        return QByteArray();
    }
}
static bool fixSize_private(QObject* obj)
{
    QWidget* w = static_cast<QWidget*>(obj);
    if(w)
    {
        if(w->isHidden())
        {
            w->setMaximumSize(w->size());
            w->setMinimumSize(w->size());
        }
        else
        {
            QWindow* pWin = w->windowHandle();
            pWin->setMaximumSize(w->size());
            pWin->setMinimumSize(w->size());
        }
        return true;
    }
    else
    {
        return false;
    }
}
static QString getNextWord_private(QString w)
{
    if(w == "A")
        return "B";
    if(w == "B")
        return "C";
    if(w == "C")
        return "D";
    if(w == "D")
        return "E";
    if(w == "E")
        return "F";
    if(w == "F")
        return "G";
    if(w == "G")
        return "H";
    if(w == "H")
        return "I";
    if(w == "I")
        return "J";
    if(w == "J")
        return "K";
    if(w == "K")
        return "L";
    if(w == "L")
        return "M";
    if(w == "M")
        return "N";
    if(w == "N")
        return "O";
    if(w == "O")
        return "P";
    if(w == "P")
        return "Q";
    if(w == "Q")
        return "R";
    if(w == "R")
        return "S";
    if(w == "S")
        return "T";
    if(w == "T")
        return "U";
    if(w == "U")
        return "V";
    if(w == "V")
        return "W";
    if(w == "W")
        return "X";
    if(w == "X")
        return "Y";
    if(w == "Y")
        return "Z";
    if(w == "Z")
        return "";
    if(w == "a")
        return "b";
    if(w == "b")
        return "c";
    if(w == "c")
        return "d";
    if(w == "d")
        return "e";
    if(w == "e")
        return "f";
    if(w == "f")
        return "g";
    if(w == "g")
        return "h";
    if(w == "h")
        return "i";
    if(w == "i")
        return "j";
    if(w == "j")
        return "k";
    if(w == "k")
        return "l";
    if(w == "l")
        return "m";
    if(w == "m")
        return "n";
    if(w == "n")
        return "o";
    if(w == "o")
        return "p";
    if(w == "p")
        return "q";
    if(w == "q")
        return "r";
    if(w == "r")
        return "s";
    if(w == "s")
        return "t";
    if(w == "t")
        return "u";
    if(w == "u")
        return "v";
    if(w == "v")
        return "w";
    if(w == "w")
        return "x";
    if(w == "x")
        return "y";
    if(w == "y")
        return "z";
    if(w == "z")
        return "";
    return "";
}
static QStringList getWindowsDrivers_private()
{
    QStringList list;
#ifdef Q_OS_WIN
    QString disk = "A";
    while(disk != "")
    {
        if(QFileInfo(disk+":/").isDir())
            list << QString(disk+":/");
        disk = getNextWord_private(disk);
    }
#endif
    return list;
}

class MyFileSystemModel: public QFileSystemModel
{
public:
    explicit MyFileSystemModel(QObject *parent = nullptr) : QFileSystemModel(parent) {}
    ~MyFileSystemModel() {}
    QIcon dirIcon() {return diricon;}
    QList<QIcon> fileIcons() {return iconlist;}
    QList<QString> fileTypes() {return formatlist;}
    void setUseFileIconProvider(bool on) {provider_using = on;}
    bool useFileIconProvider() {return provider_using;}
    bool setFileIcon(QString suffix,QIcon icon)
    {
        if(suffix.contains("."))
        {
            suffix = suffix.right(suffix.length()-suffix.lastIndexOf(".")-1);
        }
        if(!suffix.isEmpty() && !icon.isNull())
        {
            formatlist << suffix;
            iconlist << icon;
            return true;
        }
        else
        {
            return false;
        }
    }
    bool setDirIcon(QIcon icon)
    {
        if(!icon.isNull())
        {
            diricon = icon;
            return true;
        }
        else
        {
            return false;
        }
    }
    void clear()
    {
        formatlist = QList<QString>();
        iconlist = QList<QIcon>();
    }
    void setAlwaysUseFileProvider(bool on) {provider_top = on;}
    bool alwaysUseFileProvider() {return provider_top;}
    static bool multimedia_available()
    {
#if QT_MULTIMEDIA_LIB && QT_MULTIMEDIAWIDGETS_LIB
        return true;
#else
        return false;
#endif
    }
#ifdef Icon_Prepared
    void registerIcons()
    {
        QString resource_path = ":/icon/fileicons";
        QFileInfoList list = getFileList_priavte(resource_path);
        QStringList audiofilelist = {"mp3","flac","ogg","m4a","wav"};
        QStringList videofilelist = {"mp4","mkv","flv","mov"};
        QStringList imagefilelist = toStringList_private(QImageWriter::supportedImageFormats());
        QStringList photoshopfilelist = {"psd","psb"};
        for(int i = 0;i < list.count();i++)
        {
            QFileInfo info = list.at(i);
            QString filePath = info.filePath();
            QString type = filePath.left(filePath.indexOf("."));
            type = type.remove(0,QString(resource_path+"/").length());
            if(audiofilelist.contains(type))
            {
                for(QString s : audiofilelist)
                {
                    this->setFileIcon(s,QIcon(filePath));
                }
            }
            else if(videofilelist.contains(type))
            {
                for(QString s : videofilelist)
                {
                    this->setFileIcon(s,QIcon(filePath));
                }
            }
            else if(imagefilelist.contains(type))
            {
                for(QString s : imagefilelist)
                {
                    this->setFileIcon(s,QIcon(filePath));
                }
            }
            else if(photoshopfilelist.contains(type))
            {
                for(QString s : photoshopfilelist)
                {
                    this->setFileIcon(s,QIcon(filePath));
                }
            }
            else
            {
                this->setFileIcon(type,QIcon(filePath));
            }
        }
        QString diriconpath = ":/icon/folder.ico";
        this->setDirIcon(QIcon(diriconpath));
    }
#endif
private:
    bool provider_top = false;
#ifdef Q_OS_WIN
    bool provider_using = false;
#else
    bool provider_using = true;
#endif
    bool load_image_icon = false;
    //bool load_video_icon = false;
    QList<QString> formatlist;
    QList<QIcon> iconlist;
    QIcon diricon;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if(index.isValid() && role == Qt::DecorationRole && index.column() == 0)
        {
            QFileInfo info = MyFileSystemModel::fileInfo(index);
            if(provider_top)
            {
                return QFileIconProvider().icon(info);
            }
            else if(info.isDir())
            {
                if(!diricon.isNull())
                {
                    return diricon;
                }
                else if(provider_using)
                {
                    return QFileIconProvider().icon(info);
                }
            }
            else if(info.isFile())
            {
                QIcon ret;
                QString suffix = info.suffix();
                suffix = suffix.toLower();
                int current = 0;
                if(load_image_icon && QImageReader::supportedImageFormats().contains(suffix.toLatin1()) && hasFilePermission_private(info.filePath(),QIODevice::ReadOnly))
                {
#ifndef QTCONCURRENT_RUNBASE_H
                    QByteArray content = readfile_private(info.filePath());
                    QPixmap p;
                    if(p.loadFromData(content))
                    {
                        return QIcon(p);
                    }
#else
                    QFuture r = QtConcurrent::run([info](){
                        QByteArray content = readfile_private(info.filePath());
                        QPixmap p;
                        if(p.loadFromData(content))
                        {
                            return QIcon(p);
                        }
                    });
                    r.waitForFinished();
                    ret = r.result();
                    if(!ret.isNull())
                        return ret;
#endif
                }
                while(current < formatlist.count() && current < iconlist.count() && ret.isNull())
                {
                    if(suffix == formatlist.at(current))
                    {
                        ret = iconlist.at(current);
                    }
                    current++;
                }
                if(!ret.isNull())
                {
                    return ret;
                }
                else if(provider_using)
                {
                    return QFileIconProvider().icon(info);
                }
            }
        }
        return QFileSystemModel::data(index,role);
    }
};

class MyFileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MyFileDialog(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent* k);
    void resizeEvent(QResizeEvent* r);
    bool eventFilter(QObject* watched, QEvent* event);
    ~MyFileDialog();
    QString getOpenFileName(QWidget *parent = nullptr, const QString &caption = QString(),const QString &dir = QString());
    QString getExistingDirectory(QWidget *parent = nullptr, const QString &caption = QString(),const QString &dir = QString());
    bool load(QString path);
    MyFileSystemModel* fileSystemModel() {return m_fileSystemModel;}
    void setNeedDoubleClick(bool on) {need_double_click = on;}
    bool needDoubleClick() {return need_double_click;}
private slots:
    void listView_clicked(const QModelIndex &index);
private:
    MyFileSystemModel *m_fileSystemModel;
    QString m_currentPath;
    QString m_selectedFile;
    QString m_selectedPath;
    QLineEdit* pathEdit;
    QListView* listView;
    bool DirectoryMode;
    bool need_double_click;
    QPushButton* commitbutton;
    QPushButton* backbutton;
    QComboBox* pathbox;
};

#endif // MYFILEDIALOG_H
