#include "MyFileDialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
MyFileDialog::MyFileDialog(QWidget *parent) :
    QDialog(parent),
    m_fileSystemModel(new MyFileSystemModel(this))
{
#ifdef Q_Device_Mobile
    this->resize(screensize());
    need_double_click = false;
#else
    need_double_click = true;
#endif
    backbutton = new QPushButton(this);
    backbutton->move(10,0);
    backbutton->resize(40,40);
    backbutton->setIconSize(QSize(30,30));
    backbutton->setIcon(QIcon(":/icon/back.png"));
    backbutton->setStyleSheet("QPushButton{border:none;background:transparent;}");
    QObject::connect(backbutton,&QPushButton::clicked,[this](){
        QString path = m_currentPath;
        if(!path.endsWith("/"))
            path += "/";
        if(path != getlastPath_private(path))
        {
            m_currentPath = getlastPath_private(path);
            pathEdit->setText(m_currentPath);
            listView->setRootIndex(m_fileSystemModel->index(m_currentPath));
        }
        else
        {
            accept();
        }
    });

    listView = new QListView(this);
    listView->move(10,backbutton->y()+backbutton->height()+10);
#ifdef Q_Device_Mobile
    listView->resize(QSize(screenwidth()-20,screenheight()-backbutton->height()-40*2-10*3));
#else
    listView->resize(500,300);
#endif
    pathEdit = new QLineEdit(this);
    pathEdit->move(QPoint(10,listView->y()+listView->height()+10));
    pathEdit->setClearButtonEnabled(true);
    pathEdit->installEventFilter(this);
#ifdef Q_Device_Mobile
    pathEdit->resize(QSize(listView->width(),30));
#else
    pathEdit->resize(QSize(500,30));
#endif
    QObject::connect(listView,&QListView::clicked,[this](const QModelIndex &index){
        if(!need_double_click)
        {
            this->listView_clicked(index);
            //qDebug() << "send1";
        }
    });
    QObject::connect(listView,&QListView::doubleClicked,[this](const QModelIndex &index){
        if(need_double_click)
        {
            this->listView_clicked(index);
            //z  qDebug() << "send2";
        }
    });
    QObject::connect(pathEdit,&QLineEdit::returnPressed,[this](){
        if(pathEdit->text() != m_currentPath && QFileInfo(pathEdit->text()).isDir())
        {
            m_currentPath = pathEdit->text();
            pathEdit->setText(m_currentPath);
            listView->setRootIndex(m_fileSystemModel->index(m_currentPath));
        }
        else if(pathEdit->text() != m_currentPath && DirectoryMode == false && QFileInfo(pathEdit->text()).isFile())
        {
            m_selectedFile = pathEdit->text();
            accept();
        }
        else if(pathEdit->text() != m_currentPath)
        {
#ifdef Q_OS_WIN
            if(pathEdit->text() == "我的电脑")
            {
                m_currentPath = pathEdit->text();
                pathEdit->setText(m_currentPath);
                listView->setRootIndex(m_fileSystemModel->index(m_currentPath));
            }
#endif
            pathEdit->setText(m_currentPath);
        }
    });
    QObject::connect(pathEdit,&QLineEdit::editingFinished,[this](){
        pathEdit->returnPressed();
    });

    commitbutton = new QPushButton(this);
    commitbutton->resize(pathEdit->width(),40);
    commitbutton->move(pathEdit->x(),pathEdit->y()+pathEdit->height()+10);
    commitbutton->setText("选择文件夹");
    commitbutton->setStyleSheet("QPushButton{background-color:rgb(67,134,221);border: 0.5px solid rgb(67,134,221);border-radius:5;color:rgb(255,255,255);}QPushButton:pressed{background-color:#629EE4;border: 0.5px solid #629EE4;}QPushButton:hover{background-color:#629EE4;border: 0.5px solid #629EE4;}");
    QObject::connect(commitbutton,&QPushButton::clicked,[this](){
        if(DirectoryMode == true && QFileInfo(m_currentPath).isDir())
        {
            m_selectedPath = m_currentPath;
            accept();
        }
    });

    pathbox = new QComboBox(this);
    pathbox->resize(300,backbutton->height()*0.75);
    pathbox->move(backbutton->x()+backbutton->width()+10,backbutton->y()+backbutton->height()*0.5-pathbox->height()*0.5);
    QStringList list;
#ifdef Q_OS_WIN
    list << "选择常用路径" << "我的电脑" << getWindowsDrivers_private() << "桌面" << "下载" << "文档" << "当前路径";
#else
#ifdef Q_OS_ANDROID
    list << "选择常用路径" << "/" << "/sdcard" << "下载" << "文档" << "图片" << "相册" << "数据目录1（内部存储）" << "数据目录2（外部存储）" << "工作目录";
#endif
#endif
    pathbox->addItems(list);
    QObject::connect(pathbox,&QComboBox::currentTextChanged,[this](QString text){
        if(text == "选择常用路径")
        {
            return;
        }

        if(text == "桌面")
        {
            text = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
            m_currentPath = text;
            pathEdit->setText(m_currentPath);
            listView->setRootIndex(m_fileSystemModel->index(m_currentPath));
        }
        else if(text == "我的电脑")
        {
        }
        else if(text == "下载")
        {
#ifdef Q_OS_ANDROID
            text = "/sdcard/Download";
#else
            text = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
#endif
        }
        else if(text == "文档")
        {
#ifdef Q_OS_ANDROID
            text = "/sdcard/Documents";
#else
            text = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
#endif
        }
        else if(text == "图片")
        {
#ifdef Q_OS_ANDROID
            text = "/sdcard/Pictures";
#endif
        }
        else if(text == "相册")
        {
#ifdef Q_OS_ANDROID
            if(QFileInfo("/sdcard/DCIM/Camera").isDir())
                text = "/sdcard/DCIM/Camera";
            else if(QFileInfo("/sdcard/DCIM/").isDir())
                text = "/sdcard/DCIM/";
            else
                text = "/sdcard/Pictures";
#endif
        }
        else if(text == "数据目录1（内部存储）")
        {
#ifdef Q_OS_ANDROID
            text = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
#endif
        }
        else if(text == "工作目录" || text == "当前路径")
        {
            text = QCoreApplication::applicationDirPath() + "/";
        }
        else if(text == "数据目录2（外部存储）")
        {
            text = getlastPath_private(QDir::homePath());
        }

        if(QFileInfo(text).isDir())
        {
            m_currentPath = text;
            this->load(m_currentPath);
        }
        else if(QCoreApplication::applicationFilePath().endsWith(".exe") || QCoreApplication::applicationFilePath().endsWith(".EXE"))
        {
            if(text == "我的电脑")
            {
                m_currentPath = text;
                pathEdit->setText(m_currentPath);
                listView->setRootIndex(m_fileSystemModel->index(m_currentPath));
            }
        }
        pathbox->setCurrentIndex(0);
    });

#ifdef Q_OS_WIN
    //Windows
    m_currentPath = "C:/";
#else
#ifdef Q_OS_LINUX
#ifdef Q_OS_ANDROID
    //Android
    QStringList pathList;
    pathList << QCoreApplication::applicationDirPath() << QDir::homePath() << QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) << "/system/" << "/mnt/";
    for(int i = 0;i < pathList.count();i++)
    {
        this->load(pathList.at(i));
    }

    m_currentPath = "/sdcard/";
#else
    //Linux Desktop
    m_currentPath = "/home";
#endif
#else
    //Unknown
    m_currentPath = QCoreApplication::applicationDirPath() + "/";
#endif
#endif
    pathEdit->setText(m_currentPath);
    m_fileSystemModel->setRootPath(m_currentPath);
    listView->setModel(m_fileSystemModel);
    listView->setRootIndex(m_fileSystemModel->index(m_currentPath));
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listView->verticalScrollBar()->setSingleStep(60);
    //QScroller::scroller(listView)->setScrollerProperties(QScrollerProperties(QScroller::scroller(listView)->scrollerProperties()).setScrollMetric(QScrollerProperties::VerticalOvershootPolicy,QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff)));
#ifdef Q_Device_Desktop
    QScroller::grabGesture(listView->viewport(),QScroller::TouchGesture);
#else
    QScroller::grabGesture(listView->viewport(),QScroller::LeftMouseButtonGesture);
#endif

}
void MyFileDialog::keyPressEvent(QKeyEvent *k)
{
    if(k->key() == Qt::Key_Return)
        return;
    else if(k->key() == Qt::Key_Back)
        backbutton->click();
    else if(k->key() == Qt::Key_Escape)
    {
#ifdef Q_Device_Mobile
        backbutton->click();
#else
        this->close();
#endif
    }
}
void MyFileDialog::resizeEvent(QResizeEvent *r)
{
#ifdef Q_Device_Mobile
    //QMessageBox::information(this,"","screenwidth:"+QString::number(screenwidth())+"\nscreenheight:"+QString::number(screenheight())+"\nwidth:"+QString::number(this->width())+"\nheight:"+QString::number(this->height()));
    listView->resize(QSize(screenwidth()-20,screenheight()-backbutton->height()-40*2-10*3));
    listView->move(QPoint(10,backbutton->y()+backbutton->height()+10));
    pathEdit->resize(QSize(listView->width(),30));
    pathEdit->move(QPoint(10,listView->y()+listView->height()+10));
    commitbutton->resize(pathEdit->width(),40);
    commitbutton->move(pathEdit->x(),pathEdit->y()+pathEdit->height()+10);
    pathbox->resize(screenwidth()-pathbox->x()-10,pathbox->height());
#endif
}
MyFileDialog::~MyFileDialog()
{
    delete m_fileSystemModel;
}
QString MyFileDialog::getOpenFileName(QWidget *parent, const QString &caption, const QString &dir)
{
Q_UNUSED(parent)
#ifdef Q_Device_Desktop
    this->resize(pathEdit->width()+20,pathEdit->y()+pathEdit->height()+10);
    fixSize_private(this);
#endif
    DirectoryMode = false;
    commitbutton->hide();
    commitbutton->setEnabled(false);
    this->setWindowTitle(caption);
    if (!dir.isEmpty())
    {
        m_currentPath = dir;
    }
    m_fileSystemModel->setRootPath(m_currentPath);
    listView->setRootIndex(m_fileSystemModel->index(m_currentPath));
    pathEdit->setText(m_currentPath);

    // 显示对话框
    if (exec() == QDialog::Accepted)
    {
        if (!m_selectedFile.isEmpty())
        {
            return m_selectedFile;
        }
    }
    return "";
}
QString MyFileDialog::getExistingDirectory(QWidget *parent, const QString &caption, const QString &dir)
{
Q_UNUSED(parent)
#ifdef Q_Device_Desktop
    this->resize(pathEdit->width()+20,commitbutton->y()+commitbutton->height()+10);
    //fixSize_private(this,0);
#endif
    DirectoryMode = true;
    commitbutton->show();
    commitbutton->setEnabled(true);
    this->setWindowTitle(caption);
    // 设置当前目录为指定目录，如果指定目录为空，则使用当前目录
    if (!dir.isEmpty())
    {
        m_currentPath = dir;
        m_fileSystemModel->setRootPath(m_currentPath);
        listView->setRootIndex(m_fileSystemModel->index(m_currentPath));
    }
    pathEdit->setText(m_currentPath);
    // 显示对话框
    if (exec() == QDialog::Accepted)
    {
        if (!m_selectedPath.isEmpty())
        {
            return m_selectedPath;
        }
    }
    return "";
}
void MyFileDialog::listView_clicked(const QModelIndex &index)
{
    QString filePath = m_fileSystemModel->filePath(index);
    if(QFileInfo(filePath).isFile() && DirectoryMode == false)
    {
        m_selectedFile = filePath;
        accept();
    }
    else if(QFileInfo(filePath).isDir())
    {
        m_currentPath = filePath;
        this->load(m_currentPath);
        return;
    }
}
bool MyFileDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == pathEdit)
    {
        if(event->type() == QEvent::KeyRelease)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->key() == Qt::Key_Back)
            {
                QGuiApplication::inputMethod()->hide();
                //this->close();
                return true;
            }
        }
    }
    return QObject::eventFilter(watched,event);
}
bool MyFileDialog::load(QString path)
{
    if(QFileInfo(path).isDir())
    {
        m_currentPath = path;
        pathEdit->setText(m_currentPath);
        m_fileSystemModel->setRootPath(m_currentPath);
        listView->setModel(m_fileSystemModel);
        listView->setRootIndex(m_fileSystemModel->index(m_currentPath));
        return true;
    }
    else
    {
        return false;
    }
}
