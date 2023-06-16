#include "modspage.h"
#include "util/fs.h"

#include <QDir>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QProxyStyle>
#include <QHeaderView>
#include <QItemSelectionModel>

ModsPage::ModsPage(Config& config, QWidget* parent) : ConfigurationPage(config, parent) {
	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->setSpacing(20);

	useWeave = new QCheckBox(QStringLiteral("Enable Weave"));
	mainLayout->addWidget(useWeave, 0, Qt::AlignCenter);

	mods = new ModsView(this);
	mods->setModel((model = new ModsModel(config.mods, this)));

		add = new QPushButton(QStringLiteral("Add"));
		remove = new QPushButton(QStringLiteral("Remove"));
		moveUp = new QPushButton(QStringLiteral("Move Up"));
		moveDown = new QPushButton(QStringLiteral("Move Down"));

		connect(mods->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ModsPage::onSelect);

		remove->setDisabled(true);
		moveUp->setDisabled(true);
		moveDown->setDisabled(true);

		connect(add, &QPushButton::clicked, [this]() {
			QStringList files = QFileDialog::getOpenFileNames(
				nullptr,
				QStringLiteral("Open Mods Jar"),
				{},
				QStringLiteral("Weave Mod (*.jar)")
			);

			for (const QString& str : files) {
				model->addMod(QFileInfo(str));
			}

			mods->selectRow(model->rowCount(QModelIndex()) - 1);
			});

		connect(remove, &QPushButton::clicked, [this]() {
			for (const QModelIndex& item : mods->selectionModel()->selectedRows()) {
				model->removeRow(item.row());
			}
			});

		connect(moveUp, &QPushButton::clicked, [this]() {
			QModelIndexList selected = mods->selectionModel()->selectedRows();
			if (!selected.isEmpty()) {
				int currentRow = selected[0].row();
				if (currentRow > 0) {
					model->moveRow(QModelIndex(), currentRow - 1, QModelIndex(), currentRow + 1);
				}
			}
			});

		connect(moveDown, &QPushButton::clicked, [this]() {
			QModelIndexList selected = mods->selectionModel()->selectedRows();
			if (!selected.isEmpty()) {
				int currentRow = selected[0].row();
				if (currentRow < model->rowCount(QModelIndex()) - 1) {
					model->moveRow(QModelIndex(), currentRow, QModelIndex(), currentRow + 2);
				}
			}
			});

		QGridLayout* modsContainer = new QGridLayout();
		modsContainer->setSpacing(6);
		modsContainer->addWidget(mods, 0, 0, 5, 1);
		modsContainer->addWidget(add, 0, 1);
		modsContainer->addWidget(remove, 1, 1);
		modsContainer->addWidget(moveUp, 2, 1);
		modsContainer->addWidget(moveDown, 3, 1);

		mainLayout->addLayout(modsContainer, 1);


		setLayout(mainLayout);
}


QString ModsPage::title() {
	return QStringLiteral("Mods");
}

QIcon ModsPage::icon() {
	return QIcon(":/res/icons/mod.svg");
}

void ModsPage::apply() {
	config.useWeave = useWeave->isChecked();
}

void ModsPage::load() {
	useWeave->setChecked(config.useWeave);
}

void ModsPage::onSelect(const QItemSelection& selected, const QItemSelection& deselected) {
	QModelIndexList selectedRows = qobject_cast<QItemSelectionModel*>(sender())->selectedRows();

	if (selectedRows.isEmpty()) {
		remove->setDisabled(true);
		moveUp->setDisabled(true);
		moveDown->setDisabled(true);
	}
	else {
		remove->setEnabled(true);
		moveUp->setEnabled(true);
		moveDown->setEnabled(true);
	}
}

QString ModsPage::description() {
	return "List of Weave mods you want to use. Remember to enable Weave or add it in the agents tab first! Don't add any Forge/Fabric mods.";
}
