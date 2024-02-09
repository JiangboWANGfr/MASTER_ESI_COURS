#!/bin/bash

ZSH_CUSTOM=$HOME/.oh-my-zsh/custom

# Install Oh-My-Zsh if not installed
if [ ! -d "$HOME/.oh-my-zsh" ]; then
    echo "Installing Oh-My-Zsh..."
    sudo apt update
    sudo apt install zsh git -y
    sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"
fi

# Backup and modify .zshrc file
if [ -f "$HOME/.zshrc" ]; then
    cp $HOME/.zshrc $HOME/.zshrc.bak
fi

# Install Oh-My-Zsh plugins using plugin manager
echo "Installing Oh-My-Zsh plugins..."
sed -i 's/plugins=(git)/plugins=(git zsh-autosuggestions zsh-syntax-highlighting extract)/' $HOME/.zshrc
sed -i 's/ZSH_THEME=\"robbyrussell\"/ZSH_THEME=\"random\"/' $HOME/.zshrc

# Install zsh-autosuggestions
if [ ! -d "$ZSH_CUSTOM/plugins/zsh-autosuggestions" ]; then
    git clone https://github.com/zsh-users/zsh-autosuggestions $ZSH_CUSTOM/plugins/zsh-autosuggestions
fi

# Install zsh-syntax-highlighting
if [ ! -d "$ZSH_CUSTOM/plugins/zsh-syntax-highlighting" ]; then
    git clone https://github.com/zsh-users/zsh-syntax-highlighting.git $ZSH_CUSTOM/plugins/zsh-syntax-highlighting
fi

# Start Zsh
zsh