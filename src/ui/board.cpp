#include "board.hpp"
#include <cctype>
#include <QPainter>
#include <QResizeEvent>
#include <cwchar>
#include <QPixmap>
#include <QDebug>
#include <qcolor.h>

Board::Board(QWidget *parent) : QWidget(parent) {
    loadFEN("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
    setMinimumSize(BOARD_SIZE, BOARD_SIZE);
}

// draw position
void Board::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    painter.setPen(Qt::NoPen);

    int squareWidth = width() / 8;
    int squareHeight = height() / 8;

    // draw board
    for (int row = 0; row < 8; ++row){
        for (int col = 0; col < 8; ++col){
            bool isDark = (row + col) % 2;
            painter.setBrush(isDark ? darkColor : lightColor);
            painter.drawRect(col * squareWidth, row * squareHeight, squareWidth, squareHeight);
        }
    }

    // draw pieces
    int pieceWidth = squareWidth;
    int pieceHeight = squareHeight;
    for (const auto& piece : pieces){

        int x = piece.col * squareWidth + (squareWidth - pieceWidth) / 2;
        int y = piece.row * squareHeight + (squareHeight - pieceHeight) / 2;

        QRect target(x, y, pieceWidth, pieceHeight);
        painter.drawPixmap(target, piece.pixmap.scaled(pieceWidth, pieceHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // draw dragging piece
    if (isDragging && !selectedPixmap.isNull()) {
        int pieceWidth = width() / 8;
        int pieceHeight = height() / 8;

        QPoint topLeft(dragPosition.x() - pieceWidth / 2, dragPosition.y() - pieceHeight / 2);
        QRect target(topLeft, QSize(pieceWidth, pieceHeight));

        painter.drawPixmap(target, selectedPixmap.scaled(pieceWidth, pieceHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void Board::loadFEN(std::string str){
    std::string board = str.substr(0, str.find(" "));

    int file = 0; 
    int rank = 0;
    
    for (int i = 0; i < 64; ++i){
        state[i] = Piece::None;
    }

    for (int i = 0; i < board.size(); ++i){
        char ch = board[i];
        if (ch == '/'){
            rank++;
            file = 0;
        }
        else if (isdigit(ch)){
            file += ch - '0';
        }
        else{
            int color = isupper(ch) ? Piece::White : Piece::Black;
            int piece = symbolToType(tolower(ch));
            state[(rank * 8) + file] = color | piece;
            file++;
        }
        updateBoard();
    }
}

int Board::symbolToType(char ch){
    switch (ch){
        case 'p':    return Piece::Pawn;
        case 'n':    return Piece::Knight;
        case 'b':    return Piece::Bishop;
        case 'r':    return Piece::Rook;
        case 'q':    return Piece::Queen;
        case 'k':    return Piece::King;
        default:     return Piece::None;
    }
}

QPixmap Board::getPieceImg(int piece){
    int type = piece & Piece::PieceMask;
    int color = piece & Piece::ColorMask;

    QString imgPath = "";

    if (color == Piece::Black){
        imgPath += ":/assets/b";
    }
    else if (color == Piece::White){
        imgPath += ":/assets/w";
    }

    switch (type){
        case Piece::Pawn:    imgPath += "Pawn.png"; break;
        case Piece::Knight:  imgPath += "Knight.png"; break;
        case Piece::Bishop:  imgPath += "Bishop.png"; break;
        case Piece::Rook:    imgPath += "Rook.png"; break;
        case Piece::Queen:   imgPath += "Queen.png"; break;
        case Piece::King:    imgPath += "King.png"; break;
    }

    QPixmap pix(imgPath);
    return pix;
}



void Board::updateBoard(){

   pieces.clear();

   for (int i = 0; i < 64; ++i){
        QPixmap pix = getPieceImg(state[i]);
        if (!pix.isNull()) {
            int rank = i / 8;
            int file = i % 8;
            pieces.push_back({pix, rank, file});
        }
   } 
   update();
}


int Board::getIndex(const QPointF& point){

    int rank = point.y() / (static_cast<int>(width() / 8));
    int file = point.x() / (static_cast<int>(height() / 8));

    return (rank * 8) + file;
}

void Board::selectPiece(const QPointF& point){

    int idx = getIndex(point);

    QPixmap pix = getPieceImg(state[idx]);
    if (!pix.isNull()) {
        selectedPixmap = pix;
        isDragging = true;
        dragPosition = point;
        selectedPieceOriginalIndex = idx;
        selectedPiece = state[idx];
        state[idx] = Piece::None;

        updateBoard();
    }
}

void Board::placePiece(const QPointF& point){
    if (isDragging){
        int idx = getIndex(point);

        state[idx] = selectedPiece;
        selectedPiece = Piece::None;
        selectedPixmap = QPixmap();
        isDragging = false;
        selectedPieceOriginalIndex = -1;

        updateBoard();
    }
}

// Qt event handling
void Board::mousePressEvent(QMouseEvent* event){

    if (event->button() == Qt::LeftButton){
        selectPiece(event->pos());
    }

    QWidget::mousePressEvent(event);
}

void Board::mouseReleaseEvent(QMouseEvent* event){

    placePiece(event->pos());
    QWidget::mouseReleaseEvent(event);
}

void Board::mouseMoveEvent(QMouseEvent* event){

    if (isDragging) {
        dragPosition = event->pos();
        update();
    }
    QWidget::mouseMoveEvent(event);
}
