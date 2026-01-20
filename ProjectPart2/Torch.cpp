#include "Torch.h"
#include "player.h"
#include "Screen.h"

void Torch::handleCollision(player& p, point& body, Screen& currScreen, const point& target_pos) {
    
    if (p.getItemType() == ItemType::EMPTY) {
        // if the player is not holding something he pick up the torch
        p.updateItemType(ItemType::TORCH);
        p.changeHeld();
        currScreen.setCharCurrent(target_pos, ' ');
        body.draw(' '); 
        body.move();    
        body.draw();
    }

    else {
        // if the player is holding something he can't pick up the torch
        
        currScreen.setCharCurrent(body, ' ');
        body.draw(' ');

        point old_pos = body;
        body.draw(' ');
        body.move();
        body.draw();
        currScreen.setCharCurrent(body, player::TORCH);
        old_pos.draw(player::TORCH); 
    }
}