#include "TvShowEpisode.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QTime>

TvShowEpisode::TvShowEpisode(QStringList files, TvShow *parent) :
    QObject(parent)
{
    m_files = files;
    m_parent = parent;
    m_season = -2;
    m_episode = -2;
    m_playCount = 0;
    m_rating = 0;
    m_thumbnailImageChanged = false;
}

void TvShowEpisode::moveToMainThread()
{
    moveToThread(QApplication::instance()->thread());
}

void TvShowEpisode::clear()
{
    m_showTitle = "";
    m_rating = 0;
    m_season = -2;
    m_episode = -2;
    m_overview = "";
    m_writers.clear();
    m_directors.clear();
    m_playCount = 0;
    m_lastPlayed = QDateTime(QDate(2000, 02, 30), QTime(0, 0)); // invalid date
    m_firstAired = QDate(2000, 02, 30); // invalid date;
    m_certification = "";
    m_network = "";
    m_thumbnail = QUrl();
    m_thumbnailImageChanged = false;
}

bool TvShowEpisode::loadData(MediaCenterInterface *mediaCenterInterface)
{
    bool infoLoaded = mediaCenterInterface->loadTvShowEpisode(this);
    if (!infoLoaded) {
        if (this->files().count() > 0) {
            QFileInfo fi(this->files().at(0));
            this->setName(fi.completeBaseName().replace(".", " ").replace("_", " "));
        }
    }
    m_infoLoaded = infoLoaded;
    return infoLoaded;
}

void TvShowEpisode::loadData(QString id, TvScraperInterface *tvScraperInterface)
{
    tvScraperInterface->loadTvShowEpisodeData(id, this);
}

void TvShowEpisode::scraperLoadDone()
{
    emit sigLoaded();
}

bool TvShowEpisode::saveData(MediaCenterInterface *mediaCenterInterface)
{
    bool saved = mediaCenterInterface->saveTvShowEpisode(this);
    if (!m_infoLoaded)
        m_infoLoaded = saved;
    return saved;
}

TvShow *TvShowEpisode::tvShow()
{
    return m_parent;
}

bool TvShowEpisode::isValid() const
{
    return !m_files.isEmpty();
}

void TvShowEpisode::loadImages(MediaCenterInterface *mediaCenterInterface)
{
    mediaCenterInterface->loadTvShowEpisodeImages(this);
}

/*** GETTER ***/

QString TvShowEpisode::name() const
{
    return m_name;
}

QString TvShowEpisode::completeEpisodeName() const
{
    if (m_infoLoaded)
        return QString("S%1E%2 %3").arg(seasonString())
                                   .arg(episodeString())
                                   .arg(name());
    return name();
}

QStringList TvShowEpisode::files() const
{
    return m_files;
}

QString TvShowEpisode::showTitle() const
{
    if (!m_showTitle.isEmpty())
        return m_showTitle;
    if (m_parent)
        return m_parent->showTitle();

    return QString();
}

qreal TvShowEpisode::rating() const
{
    return m_rating;
}

int TvShowEpisode::season() const
{
    if (m_season == -2 && files().count() > 0) {
        QString filename = files().at(0).split(QDir::separator()).last();
        QRegExp rx("S(\\d+)E", Qt::CaseInsensitive);
        if (rx.indexIn(filename) != -1)
            return rx.cap(1).toInt();
    }
    return m_season;
}

QString TvShowEpisode::seasonString() const
{
    if (season() == -2)
        return QString("xx");
    return QString("%1").arg(season()).prepend((season() < 10) ? "0" : "");
}

int TvShowEpisode::episode() const
{
    if (m_episode == -2 && files().count() > 0) {
        QString filename = files().at(0).split(QDir::separator()).last();
        QRegExp rx("S(\\d+)E(\\d+)", Qt::CaseInsensitive);
        if (rx.indexIn(filename) != -1)
            return rx.cap(2).toInt();
    }
    return m_episode;
}

QString TvShowEpisode::episodeString() const
{
    if (episode() == -2)
        return QString("xx");
    return QString("%1").arg(episode()).prepend((episode() < 10) ? "0" : "");
}

QString TvShowEpisode::overview() const
{
    return m_overview;
}

QStringList TvShowEpisode::writers() const
{
    return m_writers;
}

QStringList TvShowEpisode::directors() const
{
    return m_directors;
}

int TvShowEpisode::playCount() const
{
    return m_playCount;
}

QDateTime TvShowEpisode::lastPlayed() const
{
    return m_lastPlayed;
}

QDate TvShowEpisode::firstAired() const
{
    return m_firstAired;
}

QString TvShowEpisode::certification() const
{
    if (!m_certification.isEmpty())
        return m_certification;
    if (m_parent)
        return m_parent->certification();

    return QString();
}

QString TvShowEpisode::network() const
{
    if (!m_network.isEmpty())
        return m_network;
    if (m_parent)
        return m_parent->network();

    return QString();
}

QUrl TvShowEpisode::thumbnail() const
{
    return m_thumbnail;
}

QImage *TvShowEpisode::thumbnailImage()
{
    return &m_thumbnailImage;
}

bool TvShowEpisode::thumbnailImageChanged() const
{
    return m_thumbnailImageChanged;
}

/*** SETTER ***/

void TvShowEpisode::setName(QString name)
{
    m_name = name;
}

void TvShowEpisode::setShowTitle(QString showTitle)
{
    m_showTitle = showTitle;
}

void TvShowEpisode::setRating(qreal rating)
{
    m_rating = rating;
}

void TvShowEpisode::setSeason(int season)
{
    m_season = season;
}

void TvShowEpisode::setEpisode(int episode)
{
    m_episode = episode;
}

void TvShowEpisode::setOverview(QString overview)
{
    m_overview = overview;
}

void TvShowEpisode::setWriters(QStringList writers)
{
    m_writers = writers;
}

void TvShowEpisode::addWriter(QString writer)
{
    m_writers.append(writer);
}

void TvShowEpisode::addDirector(QString director)
{
    m_directors.append(director);
}

void TvShowEpisode::setDirectors(QStringList directors)
{
    m_directors = directors;
}

void TvShowEpisode::setPlayCount(int playCount)
{
    m_playCount = playCount;
}

void TvShowEpisode::setLastPlayed(QDateTime lastPlayed)
{
    m_lastPlayed = lastPlayed;
}

void TvShowEpisode::setFirstAired(QDate firstAired)
{
    m_firstAired = firstAired;
}

void TvShowEpisode::setCertification(QString certification)
{
    m_certification = certification;
}

void TvShowEpisode::setNetwork(QString network)
{
    m_network = network;
}

void TvShowEpisode::setThumbnail(QUrl url)
{
    m_thumbnail = url;
}

void TvShowEpisode::setThumbnailImage(QImage thumbnail)
{
    m_thumbnailImage = thumbnail;
    m_thumbnailImageChanged = true;
}

/*** DEBUG ***/

QDebug operator<<(QDebug dbg, const TvShowEpisode &episode)
{
    QString nl = "\n";
    QString out;
    out.append("TvShowEpisode").append(nl);
    out.append(QString("  Files:         ").append(nl));
    foreach (const QString &file, episode.files())
        out.append(QString("    %1").arg(file).append(nl));
    out.append(QString("  Name:          ").append(episode.name()).append(nl));
    out.append(QString("  ShowTitle:     ").append(episode.showTitle()).append(nl));
    out.append(QString("  Season:        %1").arg(episode.season()).append(nl));
    out.append(QString("  Episode:       %1").arg(episode.episode()).append(nl));
    out.append(QString("  Rating:        %1").arg(episode.rating()).append(nl));
    out.append(QString("  FirstAired:    ").append(episode.firstAired().toString("yyyy-MM-dd")).append(nl));
    out.append(QString("  LastPlayed:    ").append(episode.lastPlayed().toString("yyyy-MM-dd")).append(nl));
    out.append(QString("  Playcount:     %1%2").arg(episode.playCount()).arg(nl));
    out.append(QString("  Certification: ").append(episode.certification()).append(nl));
    out.append(QString("  Overview:      ").append(episode.overview())).append(nl);
    foreach (const QString &writer, episode.writers())
        out.append(QString("  Writer:        ").append(writer)).append(nl);
    foreach (const QString &director, episode.directors())
        out.append(QString("  Director:      ").append(director)).append(nl);
    /*
    foreach (const QString &studio, movie.studios())
        out.append(QString("  Studio:         ").append(studio)).append(nl);
    foreach (const QString &country, movie.countries())
        out.append(QString("  Country:       ").append(country)).append(nl);
    foreach (const Actor &actor, movie.actors()) {
        out.append(QString("  Actor:         ").append(nl));
        out.append(QString("    Name:  ").append(actor.name)).append(nl);
        out.append(QString("    Role:  ").append(actor.role)).append(nl);
        out.append(QString("    Thumb: ").append(actor.thumb)).append(nl);
    }
    foreach (const Poster &poster, movie.posters()) {
        out.append(QString("  Poster:       ")).append(nl);
        out.append(QString("    ID:       ").append(poster.id)).append(nl);
        out.append(QString("    Original: ").append(poster.originalUrl.toString())).append(nl);
        out.append(QString("    Thumb:    ").append(poster.thumbUrl.toString())).append(nl);
    }
    foreach (const Poster &backdrop, movie.backdrops()) {
        out.append(QString("  Backdrop:       ")).append(nl);
        out.append(QString("    ID:       ").append(backdrop.id)).append(nl);
        out.append(QString("    Original: ").append(backdrop.originalUrl.toString())).append(nl);
        out.append(QString("    Thumb:    ").append(backdrop.thumbUrl.toString())).append(nl);
    }
    */
    dbg.nospace() << out;
    return dbg.maybeSpace();
}

QDebug operator<<(QDebug dbg, const TvShowEpisode *episode)
{
    dbg.nospace() << *episode;
    return dbg.space();
}