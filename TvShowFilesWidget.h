#ifndef TVSHOWFILESWIDGET_H
#define TVSHOWFILESWIDGET_H

#include <QModelIndex>
#include <QWidget>
#include "Globals.h"
#include "data/TvShow.h"
#include "data/TvShowEpisode.h"
#include "data/TvShowProxyModel.h"
#include "data/TvShowDelegate.h"

namespace Ui {
class TvShowFilesWidget;
}

class TvShowFilesWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit TvShowFilesWidget(QWidget *parent = 0);
    ~TvShowFilesWidget();
    void renewModel();
    void setFilter(QString filter);
    void enableRefresh();
    void disableRefresh();

public slots:
    void startSearch();

signals:
    void setRefreshButtonEnabled(bool, MainWidgets);
    void sigEpisodeSelected(TvShowEpisode *episode);
    void sigTvShowSelected(TvShow *show);
    void sigNothingSelected();

private slots:
    void searchFinished();
    void onItemActivated(QModelIndex index, QModelIndex previous);
    void onItemClicked(QModelIndex index);

private:
    Ui::TvShowFilesWidget *ui;
    TvShowProxyModel *m_tvShowProxyModel;
    TvShowDelegate *m_tvShowDelegate;
};

#endif // TVSHOWFILESWIDGET_H