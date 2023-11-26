.SILENT:
all: Client Serveur CreationBD
Client: mainclient.o windowclient.o moc_windowclient.o protocoles/TCP.o protocoles/OVESP.o protocoles/API_bd.o
    echo "Creation de l'exe Client"
    g++ -Wno-unused-parameter -o Client mainclient.o windowclient.o moc_windowclient.o protocoles/TCP.o protocoles/OVESP.o protocoles/API_bd.o /usr/lib64/libQt5Widgets.so /usr/lib64/libQt5Gui.so /usr/lib64/libQt5Core.so /usr/lib64/libGL.so -lpthread
Serveur: serveur.c protocoles/OVESP.o protocoles/TCP.o protocoles/API_bd.o
    echo "Creation de l'exe Serveur"
    gcc serveur.c protocoles/OVESP.o protocoles/TCP.o protocoles/API_bd.o -o Serveur -I/usr/include/mysql -m64 -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl
protocoles/API_bd.o: protocoles/API_bd.c protocoles/API_bd.h
    echo "Creation de l'objet protocoles/API_bd.o"
    gcc -c -o protocoles/API_bd.o protocoles/API_bd.c protocoles/API_bd.h -I/usr/include/mysql -m64 -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl
CreationBD: CreationBD.cpp
    echo "Creation de CreationBD exe"
    g++ -o CreationBD CreationBD.cpp -I/usr/include/mysql -m64 -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl
mainclient.o: mainclient.cpp
    echo "Creation de l'objet mainclient.o"
    g++ -Wno-unused-parameter -c -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -DQT_DEPRECATED_WARNINGS -DQT_QML_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I../UNIX_DOSSIER_FINAL -I. -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtWidgets -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -I. -I. -I/usr/lib64/qt5/mkspecs/linux-g++ -o mainclient.o mainclient.cpp
windowclient.o: windowclient.cpp windowclient.h protocoles/OVESP.h protocoles/TCP.h
    echo "Creation de l'objet windowclient.o"
    g++ -Wno-unused-parameter -c -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -DQT_DEPRECATED_WARNINGS -DQT_QML_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I../UNIX_DOSSIER_FINAL -I. -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtWidgets -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -I. -I. -I/usr/lib64/qt5/mkspecs/linux-g++ -o windowclient.o windowclient.cpp windowclient.h protocoles/OVESP.h protocoles/TCP.h
moc_windowclient.o: moc_windowclient.cpp
    echo "Creation de l'objet moc_windowclient.o"
    g++ -Wno-unused-parameter -c -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -DQT_DEPRECATED_WARNINGS -DQT_QML_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I../UNIX_DOSSIER_FINAL -I. -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtWidgets -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -I. -I. -I/usr/lib64/qt5/mkspecs/linux-g++ -o moc_windowclient.o moc_windowclient.cpp
protocoles/TCP.o: protocoles/TCP.h protocoles/TCP.c
    echo "Creation de l'objet protocoles/TCP.o"
    gcc -Wno-unused-parameter -c -pipe -g -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -W -D_REENTRANT -fPIC -I../UNIX_DOSSIER_FINAL -I. -o protocoles/TCP.c protocoles/TCP.h -lresolv
protocoles/OVESP.o: protocoles/OVESP.h protocoles/OVESP.c
    echo "Creation de l'objet protocoles/OVESP.o"
    gcc -Wno-unused-parameter -c -pipe -g -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -W -D_REENTRANT -fPIC -I../UNIX_DOSSIER_FINAL -I. -I/usr/include/qt5 -I/usr/include/qt5/QtWidgets -I/usr/include/qt5/QtGui -I/usr/include/qt5/QtCore -o protocoles/OVESP.h protocoles/OVESP.c