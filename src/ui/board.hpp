#ifndef BOARD_HPP
#define BOARD_HPP

#include <QWidget>
#include <QGraphicsScene>

#define BOARD_SIZE 512


class Board : public QWidget {
    Q_OBJECT

public:
    explicit Board(QWidget *parent = nullptr);
    void updateBoard(int *state);

protected:
    void paintEvent(QPaintEvent *event) override;
    void initPieces();
    void fen();

private:
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

    struct PieceImage {
        QPixmap pixmap;
        int row;
        int col;
    };

    QVector<PieceImage> pieces;

    QColor lightColor = QColor(240, 220, 180);
    QColor darkColor = QColor(180, 140, 100);

};

#endif

