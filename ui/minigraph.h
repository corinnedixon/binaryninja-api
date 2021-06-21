#pragma once

#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>
#include <QtWidgets/QWidget>

#include "binaryninjaapi.h"
#include "dockhandler.h"
#include "uitypes.h"

class ContextMenuManager;
class FlowGraphWidget;
class Menu;
class ViewFrame;

class BINARYNINJAUIAPI MiniGraph : public QWidget, public DockContextHandler
{
	Q_OBJECT
	Q_INTERFACES(DockContextHandler)

	ViewFrame* m_frame;
	FlowGraphWidget* m_flowGraphWidget = nullptr;

 public:
	MiniGraph(ViewFrame* frame);
	~MiniGraph();

	virtual void notifyViewChanged(ViewFrame* frame) override;
	virtual void notifyVisibilityChanged(bool visible) override;
	virtual bool shouldBeVisible(ViewFrame* frame) override;

 protected:
	virtual void contextMenuEvent(QContextMenuEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void scrollTo(int x, int y);

 public Q_SLOTS:
	void notifyUpdate();
};
