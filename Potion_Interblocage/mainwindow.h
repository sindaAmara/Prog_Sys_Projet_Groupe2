#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QElapsedTimer>
#include "marmite.h"

class QProgressBar;
class QTableWidget;
class QPlainTextEdit;
class QPushButton;
class QSpinBox;
class QLabel;
class QTimer;
class Personnage;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void demarrer();
    void arreter();
    void onEtatChange(int id, int etat, const QString &texte);
    void onJournal(const QString &message);
    void onLouchesChangees(int louches);
    void verifierInterblocage();

private:
    void connecter(Personnage *p);

    QProgressBar   *m_jauge;
    QTableWidget   *m_table;
    QPlainTextEdit *m_journal;
    QPushButton    *m_btnDemarrer;
    QPushButton    *m_btnArreter;
    QSpinBox       *m_nbGaulois;
    QLabel         *m_alerte;
    QTimer         *m_surveillance;

    Marmite *m_marmite = nullptr;
    QVector<Personnage *> m_personnages;   // [0] = Astérix, [1..n] = Gaulois
    QVector<int> m_etats;
    QElapsedTimer m_inactivite;
    bool m_interblocageSignale = false;
};

#endif // MAINWINDOW_H