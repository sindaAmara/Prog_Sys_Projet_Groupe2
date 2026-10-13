#include "mainwindow.h"
#include "gaulois.h"
#include "asterix.h"

#include <QCloseEvent>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>
#include <QDebug>

static const QStringList NOMS_GAULOIS = {
    "Obélix", "Abraracourcix", "Assurancetourix",
    "Cétautomatix", "Ordralfabétix", "Agecanonix"
};

static QColor couleurEtat(int etat)
{
    switch (etat) {
    case AttendMarmite: return QColor("#FFE0A3");   // orange clair
    case AttendPotion:  return QColor("#FFB3B3");   // rouge clair
    case Boit:          return QColor("#B8F0B8");   // vert
    case Combat:        return QColor("#B3D4FF");   // bleu
    case Dort:          return QColor("#DDDDDD");   // gris
    case Prepare:       return QColor("#E2C6FF");   // violet
    default:            return QColor("#FFFFFF");
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QString("Le banquet des Gaulois — ") + NOM_VERSION);
    resize(820, 640);

    auto *central = new QWidget(this);
    auto *layout  = new QVBoxLayout(central);

    auto *titre = new QLabel(QString("<h2>La marmite de potion magique</h2>"
                                     "<i>Dining Savages Problem — %1</i>").arg(NOM_VERSION));
    layout->addWidget(titre);

    // Barre de contrôle
    auto *controles = new QHBoxLayout;
    controles->addWidget(new QLabel("Nombre de Gaulois :"));
    m_nbGaulois = new QSpinBox;
    m_nbGaulois->setRange(1, NB_GAULOIS_MAX);
    m_nbGaulois->setValue(4);
    controles->addWidget(m_nbGaulois);
    m_btnDemarrer = new QPushButton("Démarrer le banquet");
    m_btnArreter  = new QPushButton("Arrêter");
    m_btnArreter->setEnabled(false);
    controles->addWidget(m_btnDemarrer);
    controles->addWidget(m_btnArreter);
    controles->addStretch();
    layout->addLayout(controles);

    // Jauge de la marmite
    m_jauge = new QProgressBar;
    m_jauge->setRange(0, M);
    m_jauge->setValue(M);
    m_jauge->setFormat(QString("Marmite : %v / %1 louches").arg(M));
    m_jauge->setMinimumHeight(28);
    layout->addWidget(m_jauge);

    // Tableau des personnages
    m_table = new QTableWidget(0, 2);
    m_table->setHorizontalHeaderLabels({"Personnage", "État"});
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionMode(QAbstractItemView::NoSelection);
    m_table->setMinimumHeight(250);
    layout->addWidget(m_table, 2);

    // Alerte interblocage (cachée au départ)
    m_alerte = new QLabel;
    m_alerte->setWordWrap(true);
    m_alerte->setStyleSheet("QLabel { background:#C62828; color:white; padding:8px; "
                            "font-weight:bold; border-radius:4px; }");
    m_alerte->hide();
    layout->addWidget(m_alerte);

    // Journal
    layout->addWidget(new QLabel("Journal du village :"));
    m_journal = new QPlainTextEdit;
    m_journal->setReadOnly(true);
    m_journal->setMaximumBlockCount(500);
    layout->addWidget(m_journal, 3);

    setCentralWidget(central);

    m_surveillance = new QTimer(this);
    m_surveillance->setInterval(250);

    connect(m_btnDemarrer, &QPushButton::clicked, this, &MainWindow::demarrer);
    connect(m_btnArreter,  &QPushButton::clicked, this, &MainWindow::arreter);
    connect(m_surveillance, &QTimer::timeout, this, &MainWindow::verifierInterblocage);
}

MainWindow::~MainWindow()
{
    arreter();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    arreter();
    event->accept();
}

void MainWindow::connecter(Personnage *p)
{
    // Les signaux sont émis depuis les threads : Qt les transmet
    // automatiquement au thread de la fenêtre.
    connect(p, &Personnage::etatChange,      this, &MainWindow::onEtatChange);
    connect(p, &Personnage::journal,         this, &MainWindow::onJournal);
    connect(p, &Personnage::louchesChangees, this, &MainWindow::onLouchesChangees);
}

void MainWindow::demarrer()
{
    arreter();

    m_journal->clear();
    m_alerte->hide();
    m_interblocageSignale = false;

    m_marmite = new Marmite;                       // UNE seule marmite
    m_jauge->setValue(m_marmite->louches);

    const int n = m_nbGaulois->value();
    m_table->setRowCount(n + 1);
    m_etats = QVector<int>(n + 1, -1);

    m_personnages.append(new Asterix(0, "Astérix (cuisinier)", m_marmite));
    for (int i = 0; i < n; ++i)
        m_personnages.append(new Gaulois(i + 1, NOMS_GAULOIS[i], m_marmite));

    for (int i = 0; i < m_personnages.size(); ++i) {
        m_table->setItem(i, 0, new QTableWidgetItem(m_personnages[i]->nom()));
        m_table->setItem(i, 1, new QTableWidgetItem("-"));
        connecter(m_personnages[i]);
    }

    onJournal("Le banquet commence ! La marmite est pleine.");
    onJournal("Panoramix est parti au congrès des druides : c'est Astérix qui prépare la potion !");

    for (Personnage *p : m_personnages)
        p->start();                                // lancement des threads

    m_inactivite.start();
    m_surveillance->start();
    m_btnDemarrer->setEnabled(false);
    m_btnArreter->setEnabled(true);
    m_nbGaulois->setEnabled(false);
}

void MainWindow::arreter()
{
    m_surveillance->stop();
    for (Personnage *p : m_personnages)
        p->requestInterruption();
    for (Personnage *p : m_personnages) {
        p->wait();
        delete p;
    }
    m_personnages.clear();
    delete m_marmite;
    m_marmite = nullptr;

    m_btnDemarrer->setEnabled(true);
    m_btnArreter->setEnabled(false);
    m_nbGaulois->setEnabled(true);
}

void MainWindow::onEtatChange(int id, int etat, const QString &texte)
{
    if (id < 0 || id >= m_table->rowCount() || id >= m_etats.size())
        return;
    m_etats[id] = etat;
    const QColor c = couleurEtat(etat);
    for (int col = 0; col < 2; ++col) {
        QTableWidgetItem *item = m_table->item(id, col);
        if (!item) continue;
        item->setBackground(c);
        item->setForeground(Qt::black);
    }
    m_table->item(id, 1)->setText(texte);
}

void MainWindow::onJournal(const QString &message)
{
    const QString ligne = QTime::currentTime().toString("hh:mm:ss.zzz  ") + message;
    m_journal->appendPlainText(ligne);
    qInfo().noquote() << ligne;   // aussi dans la console de Qt Creator
}

void MainWindow::onLouchesChangees(int louches)
{
    m_jauge->setValue(louches);
}

/*
 * Détecteur d'interblocage (pour la démonstration) :
 * si plus aucun personnage n'est "actif" (boire, combattre, préparer)
 * pendant 3 secondes, tout le monde est bloqué sur un sémaphore.
 */
void MainWindow::verifierInterblocage()
{
    if (m_personnages.isEmpty() || m_interblocageSignale)
        return;

    for (int e : m_etats) {
        if (e == Boit || e == Combat || e == Prepare || e == -1) {
            m_inactivite.restart();
            return;
        }
    }
    if (m_inactivite.elapsed() < 3000)
        return;

    m_interblocageSignale = true;

    QString detenteur;
    for (int i = 1; i < m_etats.size(); ++i)
        if (m_etats[i] == AttendPotion)
            detenteur = m_personnages[i]->nom();

    QString msg = "INTERBLOCAGE DÉTECTÉ ! Plus personne ne bouge depuis 3 s.\n";
    if (!detenteur.isEmpty())
        msg += detenteur + " détient la marmite (mutex) et attend qu'elle soit pleine (marmitePleine), "
                           "pendant qu'Astérix attend la marmite (mutex) pour la remplir : attente circulaire.";
    m_alerte->setText(msg);
    m_alerte->show();
    onJournal("*** " + msg.replace('\n', ' ') + " ***");
}