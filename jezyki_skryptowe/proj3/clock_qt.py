import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QComboBox, QPushButton, QTabWidget, QTimeEdit, QMessageBox, QAction, QMenuBar, QHBoxLayout
from PyQt5.QtCore import QTimer, QDateTime, Qt, QTime
import pytz


FONT_SIZE = 20


class ClockAppQT5(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('184657 Panfil Wojciech - Języki skryptowe 3 - Zegar')
        self.setGeometry(100, 100, 800, 600)

        # Tab Widget
        self.tab_widget = QTabWidget()
        self.clock_tab = QWidget()
        self.stopwatch_tab = QWidget()
        self.timer_tab = QWidget()

        # Clock Tab
        self.clock_layout = QVBoxLayout()
        self.current_time_label = QLabel()
        self.current_time_label.setAlignment(Qt.AlignCenter)
        self.current_time_label.font().setWeight(FONT_SIZE)
        self.clock_layout.addWidget(self.current_time_label)

        self.timezones = pytz.common_timezones

        self.timezone_combo = QComboBox()
        self.timezone_combo.addItems(self.timezones)
        self.timezone_combo.currentIndexChanged.connect(self.update_time)

        self.clock_layout.addWidget(self.timezone_combo)
        self.clock_tab.setLayout(self.clock_layout)
        self.tab_widget.addTab(self.clock_tab, "Zegar")

        # Stopwatch Tab
        self.stopwatch_layout = QVBoxLayout()
        self.stopwatch_label = QLabel("00:00:00")
        self.stopwatch_label.setAlignment(Qt.AlignCenter)
        self.stopwatch_label.font().setWeight(FONT_SIZE)
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

        # Timer Tab
        self.timer_layout = QVBoxLayout()

        self.timer_edit = QTimeEdit()
        self.timer_edit.setDisplayFormat("hh:mm:ss")
        self.timer_edit.setTime(QTime(0, 0))
        self.timer_edit.setAlignment(Qt.AlignCenter)
        self.timer_edit.font().setWeight(FONT_SIZE)
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

        self.menu_bar = QMenuBar(self)

        # File menu
        file_menu = self.menu_bar.addMenu('Plik')
        exit_action = QAction('Wyjdź', self)
        exit_action.triggered.connect(self.exit_app)
        file_menu.addAction(exit_action)

        # Help menu
        help_menu = self.menu_bar.addMenu('Pomoc')
        about_action = QAction('O aplikacji', self)
        about_action.triggered.connect(self.show_about_dialog)
        help_menu.addAction(about_action)

        layout = QVBoxLayout()
        layout.addWidget(self.menu_bar)
        layout.addWidget(self.tab_widget)
        self.setLayout(layout)

        # Clock
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_time)
        self.timer.start(1000)
        self.update_time()

        # Stopwatch
        self.stopwatch_timer = QTimer()
        self.stopwatch_timer.timeout.connect(self.tick_stopwatch)
        self.stopwatch_running = False
        self.stopwatch_seconds = 0

        # Timer
        self.timer_timer = QTimer()
        self.timer_timer.timeout.connect(self.update_timer)
        self.timer_running = False

    def update_time(self):
        current_timezone = pytz.timezone(self.timezone_combo.currentText())
        current_time = QDateTime.currentDateTime().toPyDateTime()
        local_time = current_time.astimezone(current_timezone)
        local_time_str = local_time.strftime('%Y-%m-%d %H:%M:%S %Z%z')

        # Display the local time
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
        self.timer_edit.setTime(QTime(0, 0))

    def update_timer(self):
        current_time = QDateTime.currentDateTime().time()
        if current_time >= self.timer_end_time or not self.timer_running:
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
                     "- Skorzystać ze stopera celem zmierzenia czasu.\n" \
                     "- Użyć minutnika do odmierzenia czasu. \n\n" \
                     "Stworzono używając PyQT5. \n\n"\
                     "184657 Panfil Wojciech"
        QMessageBox.about(self, 'O aplikacji', about_text)


def launch_qt():
    app = QApplication(sys.argv)
    clock_app = ClockAppQT5()
    clock_app.show()
    sys.exit(app.exec_())
