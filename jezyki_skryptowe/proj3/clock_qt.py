import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QComboBox, QPushButton, QTabWidget, \
    QTimeEdit, QMessageBox, QAction, QMenuBar, QHBoxLayout, QListWidget, QDialog, QCheckBox
from PyQt5.QtCore import QTimer, QDateTime, Qt, QTime, QDate
import pytz


FONT_SIZE = 20
ENGLISH_TO_POLISH_DAYS = {
    "Mon": "Pon",
    "Tue": "Wt",
    "Wed": "Śr",
    "Thu": "Czw",
    "Fri": "Pt",
    "Sat": "Sob",
    "Sun": "Niedz"
}


class Alarm:
    def __init__(self, time, days, enabled=True):
        self.time = time
        self.days = days
        self.enabled = enabled


class AddAlarmDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Dodaj nowy alarm")
        self.setModal(True)

        layout = QVBoxLayout()

        self.time_edit = QTimeEdit()
        self.time_edit.setDisplayFormat("HH:mm:ss")
        layout.addWidget(QLabel("Czas alarmu:"))
        layout.addWidget(self.time_edit)

        self.day_checkboxes = []
        days_of_week = ["Pon", "Wt", "Śr", "Czw", "Pt", "Sob", "Niedz"]
        for day in days_of_week:
            checkbox = QCheckBox(day)
            self.day_checkboxes.append(checkbox)
            layout.addWidget(checkbox)

        layout.addSpacing(20)

        self.enabled_checkbox = QCheckBox("Włączony")
        self.enabled_checkbox.setChecked(True)
        layout.addWidget(self.enabled_checkbox)

        buttons_layout = QHBoxLayout()
        add_button = QPushButton("Zapisz")
        add_button.clicked.connect(self.accept)
        cancel_button = QPushButton("Anuluj")
        cancel_button.clicked.connect(self.reject)
        buttons_layout.addWidget(add_button)
        buttons_layout.addWidget(cancel_button)

        layout.addLayout(buttons_layout)
        self.setLayout(layout)

    def set_alarm(self, alarm):
        self.time_edit.setTime(alarm.time)
        for checkbox, day in zip(self.day_checkboxes, alarm.days):
            if day in checkbox.text():
                checkbox.setChecked(True)
        self.enabled_checkbox.setChecked(alarm.enabled)

    def get_alarm(self):
        time = self.time_edit.time()

        days = [checkbox.text() for checkbox in self.day_checkboxes if checkbox.isChecked()]
        enabled = self.enabled_checkbox.isChecked()
        return Alarm(time, days, enabled)


class ClockAppQT5(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('184657 Panfil Wojciech - Języki skryptowe 3 - Zegar')
        self.setGeometry(100, 100, 800, 600)

        self.tab_widget = QTabWidget()
        self.clock_tab = QWidget()
        self.stopwatch_tab = QWidget()
        self.timer_tab = QWidget()

        self.clock_layout = QVBoxLayout()
        self.current_time_label = QLabel()
        self.current_time_label.setAlignment(Qt.AlignCenter)
        font = self.current_time_label.font()
        font.setPointSize(FONT_SIZE)
        self.current_time_label.setFont(font)
        self.clock_layout.addWidget(self.current_time_label)

        self.timezones = pytz.common_timezones

        self.timezone_combo = QComboBox()
        self.timezone_combo.addItems(self.timezones)
        self.timezone_combo.currentIndexChanged.connect(self.update_time)
        self.timezone_combo.view().setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)

        self.clock_layout.addWidget(self.timezone_combo)
        self.clock_tab.setLayout(self.clock_layout)
        self.tab_widget.addTab(self.clock_tab, "Zegar")

        self.stopwatch_layout = QVBoxLayout()
        self.stopwatch_label = QLabel("00:00:00")
        self.stopwatch_label.setAlignment(Qt.AlignCenter)
        self.stopwatch_label.font().setWeight(FONT_SIZE)
        font = self.stopwatch_label.font()
        font.setPointSize(FONT_SIZE)
        self.stopwatch_label.setFont(font)
        self.stopwatch_layout.addWidget(self.stopwatch_label)

        self.start_stopwatch_button = QPushButton("Rozpocznij")
        self.stop_stopwatch_button = QPushButton("Zatrzymaj")
        self.reset_stopwatch_button = QPushButton("Wyczyść")

        self.start_stopwatch_button.clicked.connect(self.start_stopwatch)
        self.stop_stopwatch_button.clicked.connect(self.stop_stopwatch)
        self.reset_stopwatch_button.clicked.connect(self.reset_stopwatch)

        stopwatch_buttons_layout = QVBoxLayout()
        stopwatch_buttons_layout.addWidget(self.start_stopwatch_button)
        stopwatch_buttons_layout.addWidget(self.stop_stopwatch_button)
        stopwatch_buttons_layout.addWidget(self.reset_stopwatch_button)

        self.stopwatch_layout.addLayout(stopwatch_buttons_layout)

        self.stopwatch_tab.setLayout(self.stopwatch_layout)
        self.tab_widget.addTab(self.stopwatch_tab, "Stoper")

        self.timer_layout = QVBoxLayout()
        self.timer_edit = QTimeEdit()
        self.timer_edit.setDisplayFormat("hh:mm:ss")
        self.timer_edit.setTime(QTime(0, 0))
        self.timer_edit.setAlignment(Qt.AlignCenter)
        self.timer_edit.font().setWeight(FONT_SIZE)
        font = self.timer_edit.font()
        font.setPointSize(FONT_SIZE)
        self.timer_edit.setFont(font)
        self.timer_layout.addWidget(self.timer_edit, alignment=Qt.AlignCenter)

        self.start_timer_button = QPushButton("Rozpocznij")
        self.stop_timer_button = QPushButton("Zatrzymaj")
        self.reset_timer_button = QPushButton("Wyczyść")

        self.start_timer_button.clicked.connect(self.start_timer)
        self.stop_timer_button.clicked.connect(self.stop_timer)
        self.reset_timer_button.clicked.connect(self.reset_timer)

        timer_buttons_layout = QVBoxLayout()
        timer_buttons_layout.addWidget(self.start_timer_button)
        timer_buttons_layout.addWidget(self.stop_timer_button)
        timer_buttons_layout.addWidget(self.reset_timer_button)

        self.timer_layout.addLayout(timer_buttons_layout)

        self.timer_tab.setLayout(self.timer_layout)
        self.tab_widget.addTab(self.timer_tab, "Minutnik")

        self.alarm_tab = QWidget()
        self.alarm_layout = QVBoxLayout()

        self.alarms = []
        self.alarm_list = QListWidget()
        self.alarm_list.itemDoubleClicked.connect(self.edit_selected_alarm)
        self.alarm_layout.addWidget(self.alarm_list)

        alarm_buttons_layout = QVBoxLayout()
        self.add_alarm_button = QPushButton("Dodaj nowy alarm")
        self.remove_alarm_button = QPushButton("Usuń alarm")

        self.add_alarm_button.clicked.connect(self.add_new_alarm_dialog)
        self.remove_alarm_button.clicked.connect(self.remove_selected_alarm)

        alarm_buttons_layout.addWidget(self.add_alarm_button)
        alarm_buttons_layout.addWidget(self.remove_alarm_button)
        self.alarm_layout.addLayout(alarm_buttons_layout)

        self.alarm_tab.setLayout(self.alarm_layout)
        self.tab_widget.addTab(self.alarm_tab, "Budzik")

        self.menu_bar = QMenuBar(self)

        file_menu = self.menu_bar.addMenu('Plik')
        exit_action = QAction('Wyjdź', self)
        exit_action.triggered.connect(self.exit_app)
        file_menu.addAction(exit_action)

        help_menu = self.menu_bar.addMenu('Pomoc')
        about_action = QAction('O aplikacji', self)
        about_action.triggered.connect(self.show_about_dialog)
        help_menu.addAction(about_action)

        layout = QVBoxLayout()
        layout.addWidget(self.menu_bar)
        layout.addWidget(self.tab_widget)
        self.setLayout(layout)

        self.timer = QTimer()
        self.timer.timeout.connect(self.update_time)
        self.timer.start(1000)
        self.update_time()

        self.stopwatch_timer = QTimer()
        self.stopwatch_timer.timeout.connect(self.tick_stopwatch)
        self.stopwatch_running = False
        self.stopwatch_seconds = 0

        self.timer_timer = QTimer()
        self.timer_timer.timeout.connect(self.update_timer)
        self.timer_running = False
        self.timer_end_time = QDateTime.currentDateTime().time()

        self.check_alarm_timer = QTimer()
        self.check_alarm_timer.timeout.connect(self.check_alarms)
        self.check_alarm_timer.start(1000)

    def update_time(self):
        current_timezone = pytz.timezone(self.timezone_combo.currentText())
        current_time = QDateTime.currentDateTime().toPyDateTime()
        local_time = current_time.astimezone(current_timezone)
        local_time_str = local_time.strftime('%Y-%m-%d %H:%M:%S %Z%z')

        self.current_time_label.setText(local_time_str)

    def start_stopwatch(self):
        self.stopwatch_running = True
        self.stopwatch_timer.start(1000)

    def stop_stopwatch(self):
        self.stopwatch_running = False
        self.stopwatch_timer.stop()

    def reset_stopwatch(self):
        self.stopwatch_running = False
        self.stopwatch_timer.stop()
        self.stopwatch_seconds = 0
        self.update_stopwatch()

    def tick_stopwatch(self):
        self.stopwatch_seconds += 1
        self.update_stopwatch()

    def update_stopwatch(self):
        seconds = self.stopwatch_seconds % 60
        minutes = (self.stopwatch_seconds // 60) % 60
        hours = self.stopwatch_seconds // 3600
        stopwatch_time = "{:02d}:{:02d}:{:02d}".format(hours, minutes, seconds)
        self.stopwatch_label.setText(stopwatch_time)

    def start_timer(self):
        if not self.timer_running:
            self.timer_running = True
            self.timer_end_time = QDateTime.currentDateTime().time().addSecs(QTime(0, 0).secsTo(self.timer_edit.time()))
            self.timer_timer.start(1000)

    def stop_timer(self):
        self.timer_running = False

    def reset_timer(self):
        self.timer_timer.stop()
        self.stop_timer()
        self.timer_edit.setTime(QTime(0, 0))

    def update_timer(self):
        current_time = QDateTime.currentDateTime().time()
        if not self.timer_running:
            return
        if current_time >= self.timer_end_time:
            self.stop_timer()
            self.reset_timer()
        else:
            remaining_time = current_time.secsTo(self.timer_end_time)
            display_time = QTime(0, 0).addSecs(remaining_time)
            self.timer_edit.setTime(display_time)

    def exit_app(self):
        sys.exit()

    def show_about_dialog(self):
        about_text = "Zegar\n\n" \
                     "Aplikacja pozwala na:\n" \
                     "- Wyświetlenie aktualnego czasu w różnych strefach czasowych.\n" \
                     "- Skorzystanie ze stopera celem zmierzenia czasu.\n" \
                     "- Użycie minutnika do odmierzenia czasu. \n" \
                     "- Ustawienie budzika. \n\n" \
                     "Stworzono używając PyQT5. \n\n"\
                     "184657 Panfil Wojciech"
        QMessageBox.about(self, 'O aplikacji', about_text)

    def add_new_alarm_dialog(self):
        dialog = AddAlarmDialog(self)
        if dialog.exec_():
            alarm = dialog.get_alarm()
            self.alarms.append(alarm)
            self.update_alarm_list()

    def edit_selected_alarm(self, item):
        selected_index = self.alarm_list.indexFromItem(item).row()
        alarm = self.alarms[selected_index]
        dialog = AddAlarmDialog(self)
        dialog.setWindowTitle("Edytuj alarm")
        dialog.set_alarm(alarm)
        if dialog.exec_():
            edited_alarm = dialog.get_alarm()
            self.alarms[selected_index] = edited_alarm
            self.update_alarm_list()

    def remove_selected_alarm(self):
        selected_items = self.alarm_list.selectedItems()
        if not selected_items:
            return
        alarm_index = self.alarm_list.row(selected_items[0])
        del self.alarms[alarm_index]
        self.update_alarm_list()

    def update_alarm_list(self):
        self.alarm_list.clear()
        for alarm in self.alarms:
            item = f"{alarm.time.toString("hh:mm:ss")} - {alarm.days} ({"Włączony" if alarm.enabled else "Wyłączony"})"
            self.alarm_list.addItem(item)

    def check_alarms(self):
        current_time = QTime.currentTime()
        current_day = QDate.currentDate().toString("ddd")
        for alarm in self.alarms:
            if current_time.hour() == alarm.time.hour() and \
                    current_time.minute() == alarm.time.minute() and \
                    current_time.second() == alarm.time.second() and ENGLISH_TO_POLISH_DAYS[current_day] in alarm.days and alarm.enabled:
                self.show_alarm_message(alarm)
                alarm.enabled = False
                self.update_alarm_list()

    def show_alarm_message(self, alarm):
        msg_box = QMessageBox(self)
        msg_box.setWindowTitle("Alarm!")
        msg_box.setIcon(QMessageBox.Information)
        msg_box.setText("Alarm {}!".format(alarm.time.toString("hh:mm:ss")))
        msg_box.exec_()


def launch_qt():
    app = QApplication(sys.argv)
    clock_app = ClockAppQT5()
    clock_app.show()
    sys.exit(app.exec_())
