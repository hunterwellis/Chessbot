#include <QApplication>
#include <QTimer>
#include <iostream>
#include "ui/board.hpp"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Board board;
    board.setWindowTitle("chess-bot");
    board.resize(BOARD_SIZE, BOARD_SIZE);
    board.setFixedSize(board.size());
    board.show();


    std::vector<int> vec = {32, 33, 34, 35, 36, 37};
    board.indicate(vec);
    std::cout << "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2" << std::endl;
    std::cout << board.getFEN() << std::endl;

    return app.exec();
}
