#ifndef CHESSBOT_UI_BOARD_HPP_
#define CHESSBOT_UI_BOARD_HPP_

#include <QWidget>
#include <QGraphicsScene>
#include <QVector>
#include <vector>

#define BOARD_SIZE 512

// type is indicated by 3 LSB and color by bits 5 and 6
// example: white king is defined as: Piece.White | Piece.King == 0b100001
enum Piece{
    None = 0, 
    Pawn = 1, 
    Knight = 2, 
    Bishop = 3, 
    Rook = 4, 
    Queen = 5, 
    King = 6,

    White = 8,
    Black = 16,

    PieceMask = 0b00111,
    ColorMask = 0b11000
};

class Board : public QWidget {
    Q_OBJECT

public:
    explicit Board(QWidget *parent = nullptr);
    void updateBoard();
    void indicate(std::vector<int> squares);

    void loadFEN(std::string str);
    std::string getFEN();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    // the board state
    int state[64] = { Piece::None };
    QVector<int> indicators;


    QPixmap getPieceImg(int piece);
    int getIndex(const QPointF& point);

    void selectPiece(const QPointF& point);
    void placePiece(const QPointF& point);

    int symbolToType(char ch);
    char typeToSymbol(int piece);
    
    // variables for moving/dragging piece
    QPixmap selectedPixmap;
    int selectedPiece = Piece::None;
    bool isDragging = false;
    QPointF dragPosition;
    int selectedPieceOriginalIndex = -1;

    // used to draw the board
    struct PieceImage {
        QPixmap pixmap;
        int row;
        int col;
    };

    QVector<PieceImage> pieces;

    QColor lightColor = QColor(240, 220, 160);
    QColor darkColor = QColor(180, 140, 100);
    QColor selectColor = QColor(180, 180, 180);

};

#endif

