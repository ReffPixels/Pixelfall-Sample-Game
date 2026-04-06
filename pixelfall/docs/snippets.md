
    // Chess board isometric
    Vector2 tileSize{80.0f, 40.0f};  // width:height = 2:1
    Color whiteSquareColor{Color::fromHexcode("#edd6b0")};
    Color blackSquareColor{Color::fromHexcode("#b88762")};

    // Center of the whole board
    Vector2 boardPosition{
        appWindow->getLogicalSize().x / 2.0f,
        (appWindow->getLogicalSize().y - (tileSize.y * 8)) / 2.0f
    };

    // Draw board
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            // Center of this tile in screen space
            float cx = boardPosition.x + (file - rank) * (tileSize.x / 2);
            float cy = boardPosition.y + (file + rank) * (tileSize.y / 2);

            // Four corners of the diamond
            Vector2 top{cx,         cy - (tileSize.y / 2)};
            Vector2 right{cx + (tileSize.x / 2), cy};
            Vector2 bot{cx,         cy + (tileSize.y / 2)};
            Vector2 left{cx - (tileSize.x / 2), cy};

            painter->drawQuad(
                top, right, bot, left,
                ((file + rank + 1) % 2 == 0) ? blackSquareColor : whiteSquareColor
            );
        }
    }

    
// Draws a piece on the board isometrically
void ChessPiece::draw(PieceInfo pieceInfo, Vector2 boardPosition, Vector2 tileSize,
    Painter& painter, TextureCache& textureCache, const std::filesystem::path& projectPath) {

    float scale{0.75f};
    float heightDisplacement{0.25f};
    Vector2 spriteSize{tileSize.x * scale, tileSize.y * 2 * scale};

    Vector2 physicalPosition = {
        boardPosition.x + ((float)pieceInfo.position.x - (float)pieceInfo.position.y)
        * tileSize.x / 2.0f - spriteSize.y / 2.0f,
        boardPosition.y + ((float)pieceInfo.position.x + (float)pieceInfo.position.y)
        * tileSize.y / 2.0f - (spriteSize.y / 2.0f + heightDisplacement * spriteSize.y),
    };
    Texture& pieceTexture = textureCache.loadTexture(findImagePath(pieceInfo, projectPath).string());

    painter.drawSprite(
        physicalPosition,
        spriteSize,
        pieceTexture
    );
}