#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QString>
#include <QWidget>
#include <QGraphicsDropShadowEffect>

class ThemeManager {
public:
    static ThemeManager& instance() {
        static ThemeManager inst;
        return inst;
    }

    QString getGlobalStyleSheet() const {
        return R"(
            /* --- Next-Gen Glassmorphism Design System --- */
            
            QMainWindow, QDialog { 
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #080d14, stop:1 #020617);
                color: #f1f5f9; 
            }
            
            QWidget { 
                font-family: 'Outfit', 'Inter', -apple-system, system-ui, sans-serif; 
                font-size: 11pt; 
            }

            /* Glassmorphism Containers */
            QGroupBox {
                background: rgba(30, 41, 59, 0.4);
                border: 1px solid rgba(255, 255, 255, 0.1);
                border-radius: 16px;
                margin-top: 25px;
                font-weight: 800;
                color: #818cf8;
                padding: 15px;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                left: 20px;
                padding: 0 5px;
            }

            /* Interactive Elements */
            QPushButton { 
                background: rgba(30, 41, 59, 0.6); 
                border: 1px solid rgba(255, 255, 255, 0.08); 
                border-radius: 10px; 
                padding: 8px 16px; 
                color: #e2e8f0; 
                font-weight: 700; 
            } 
            QPushButton:hover { 
                background: rgba(99, 102, 241, 0.15); 
                border: 1px solid rgba(99, 102, 241, 0.5); 
                color: #ffffff; 
            } 
            QPushButton:pressed {
                background: rgba(99, 102, 241, 0.3);
                border: 1px solid #6366f1;
            }
            QPushButton[accent='true'] { 
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #6366f1, stop:1 #4338ca); 
                color: white; 
                border: none;
            } 
            QPushButton[accent='true']:hover { 
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #818cf8, stop:1 #6366f1);
            }

            /* Premium Inputs */
            QLineEdit, QComboBox, QDateTimeEdit, QSpinBox, QTextEdit { 
                background: rgba(15, 23, 42, 0.8); 
                border: 1px solid rgba(255, 255, 255, 0.1); 
                border-radius: 10px; 
                padding: 10px; 
                color: #f1f5f9; 
            } 
            QLineEdit:focus, QComboBox:focus { 
                border: 1px solid rgba(99, 102, 241, 0.8); 
                background: rgba(15, 23, 42, 1.0); 
            } 

            /* Modern Navigation */
            QTabWidget::pane { 
                border: 1px solid rgba(255, 255, 255, 0.1); 
                top: -1px; 
                border-radius: 12px;
                background: transparent; 
            } 
            QTabBar::tab { 
                background: transparent; 
                color: #94a3b8; 
                padding: 12px 30px; 
                margin-bottom: 2px;
            } 
            QTabBar::tab:selected { 
                color: #818cf8; 
                border-bottom: 3px solid #6366f1; 
                font-weight: 800; 
                background: rgba(99, 102, 241, 0.1);
            } 

            /* Modern Tables */
            QTableView { 
                background: rgba(15, 23, 42, 0.4); 
                border: 1px solid rgba(255, 255, 255, 0.05); 
                border-radius: 16px; 
                gridline-color: rgba(255, 255, 255, 0.05);
                alternate-background-color: rgba(30, 41, 59, 0.2);
            } 
            QHeaderView::section { 
                background: rgba(30, 41, 59, 0.8); 
                color: #cbd5e1; 
                padding: 12px; 
                border: none; 
                font-weight: 800; 
                text-transform: uppercase;
            }

            /* Dashboard Specifics */
            #StatsCard {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgba(30, 41, 59, 0.6), stop:1 rgba(15, 23, 42, 0.6));
                border: 1px solid rgba(255, 255, 255, 0.1);
                border-radius: 20px;
            }
            #LiveClock {
                color: #6366f1;
                font-family: 'JetBrains Mono', 'Courier New', monospace;
                font-size: 36pt;
                font-weight: 800;
            }
        )";
    }

    void applyShadow(QWidget* widget) {
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(widget);
        shadow->setBlurRadius(20);
        shadow->setXOffset(0);
        shadow->setYOffset(10);
        shadow->setColor(QColor(0, 0, 0, 150));
        widget->setGraphicsEffect(shadow);
    }

private:
    ThemeManager() {}
};

#endif // THEMEMANAGER_H
