#pragma once

#include <QtWidgets/QListView>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QWidget>

#include "dockhandler.h"
#include "uitypes.h"
#include "viewframe.h"

//! A variable list item can represent either a function-local variable, or a
//! data variable referenced by the current function.
enum class VariableListItemType {
    LocalVariable,
    DataVariable
};

//! An item part of VariableListModel.
class VariableListItem {
    FunctionRef m_func;
    VariableListItemType m_type;
    std::string m_name;

    uint64_t m_refPoint;

    BinaryNinja::Variable m_var;
    BinaryNinja::PossibleValueSet m_pvs;
    BinaryNinja::DataVariable m_dataVar;

public:
    //! Create a new VariableListItem of the LocalVariable type.
    VariableListItem(FunctionRef func, BinaryNinja::Variable var,
        BinaryNinja::PossibleValueSet pvs, std::string name);

    //! Create a new VariableListItem of the DataVariable type.
    VariableListItem(FunctionRef func, BinaryNinja::DataVariable dataVar,
        uint64_t refPoint, std::string name);

    //! Get the type of this list item.
    VariableListItemType type() const;

    std::string name() const;
    std::string constantValue() const;
    BinaryNinja::PossibleValueSet possibleValueSet() const;
    std::vector<BinaryNinja::InstructionTextToken> tokensBeforeName() const;
    std::vector<BinaryNinja::InstructionTextToken> tokensAfterName() const;
    std::vector<BinaryNinja::InstructionTextToken> displayTokens() const;

    //! Get the represented variable; use with variable items only.
    BinaryNinja::Variable variable() const;

    //! Get the represented data variable; use with variable items only.
    BinaryNinja::DataVariable dataVariable() const;

    //! Get the first use of this variable; use with data variables items only.
    uint64_t refPoint() const;
};

//! The backing model for the variable list widget, holds VariableListItem.
class BINARYNINJAUIAPI VariableListModel : public QAbstractListModel {
    Q_OBJECT

    ViewFrame* m_view;
    BinaryViewRef m_data;
    FunctionRef m_func;
    std::vector<VariableListItem> m_vars;

    QItemSelectionModel* m_selModel;

public:
    VariableListModel(QWidget* parent, ViewFrame* view, BinaryViewRef data);

    //! Clear the list's content.
    void clear();

    //! Get the current function.
    FunctionRef function() const;

    //! Set the focused function and update the content of the list.
    void setFunction(FunctionRef func, BNFunctionGraphType il,
        HighlightTokenState hts);

    //! Set the selection model, should correspond ot the parent widget's.
    void setSelectionModel(QItemSelectionModel* model);

    // -- QAbstractListModel --

    virtual QVariant data(const QModelIndex& i, int role) const override;
    virtual QModelIndex index(int row, int col,
        const QModelIndex& parent = QModelIndex()) const override;

    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QVariant headerData(int column, Qt::Orientation orientation,
        int role) const override;
};

class VariableListItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    VariableListItemDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem& opt,
        const QModelIndex& index) const;

    QSize sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& index) const;
};

//! The main variable list dock widget.
class BINARYNINJAUIAPI VariableListView : public QWidget, public DockContextHandler {
    Q_OBJECT
    Q_INTERFACES(DockContextHandler)

    ViewFrame* m_view;
    BinaryViewRef m_data;

    VariableListModel* m_listModel;
    QListView* m_list;

public:
    VariableListView(ViewFrame* view, BinaryViewRef data);

    void updateContent();

    // Get the VariableListItem corresponding to the current selection.
    VariableListItem* selectedItem() const;

    //! Show the rename dialog for the seleected variable.
    void changeSelectedVariableName();

    //! Show the new type dialog for the seleected variable.
    void changeSelectedVariableType();

    //! Clear the selected variable's name.
    void clearSelectedVariableName();

    //! Navigate to the first usage of the selected variable.
    void showSelectedVariableFirstUsage();

    //! Navigate to the definition of the selected data variable.
    void showSelectedDataVariableDefinition();
};