import time 

import multiprocessing
multiprocessing.freeze_support()

time.sleep (1)
import os
import torch
import torch.nn as nn
import torch.optim as optim
import torchvision.transforms as transforms
import torchvision.datasets as datasets
from torch.utils.data import DataLoader,random_split
from sklearn.model_selection import train_test_split
from torchvision.datasets import ImageFolder
import torchvision.transforms as transforms
import pathlib


# Définition des hyperparamètres
batch_size = 16
learning_rate = 0.001
#num_epochs = 10
num_epochs=7

data_path = "dataset"

# Définition des transformations à appliquer aux images
train_transform = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.RandomHorizontalFlip(),
    transforms.RandomRotation(10),
    transforms.ColorJitter(brightness=0.3, contrast=0.3, saturation=0.3, hue=0.1),
    transforms.ToTensor(),
    transforms.Normalize((1,), (1,))
])

val_transform = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
    transforms.Normalize((0.5,), (0.5,))
])

# Chargement de la dataset
dataset = ImageFolder(data_path)

# Séparation en train_dataset, val_dataset et test_dataset
train_size = int(0.7 * len(dataset))
val_size = int(0.3 * len(dataset))
test_size = len(dataset) - train_size - val_size
train_dataset, val_dataset, test_dataset = random_split(dataset, [train_size, val_size, test_size])

# Application des transformations de data augmentation aux données d'entraînement et de validation
train_dataset.dataset.transform = train_transform
val_dataset.dataset.transform = val_transform

# Obtention des classes de la dataset
classes = dataset.classes

def main():
    # Chargement des DataLoader pour l'entraînement, la validation et le test
    batch_size = 32
    train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True, num_workers=4)
    val_loader = DataLoader(val_dataset, batch_size=batch_size, shuffle=False, num_workers=4)
    test_loader = DataLoader(test_dataset, batch_size=batch_size, shuffle=False, num_workers=4)

    # Création du modèle ResNet18 préentraîné et remplacement de la dernière couche linéaire pour adapter le nombre de classes
    model = torch.hub.load('pytorch/vision:v0.10.0',model='resnet18', pretrained=True)
    num_features = model.fc.in_features
    model.fc = nn.Linear(num_features, len(classes))

    # Définition de la fonction de coût et de l'optimiseur
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=learning_rate)

    # Entraînement du modèle
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    #device = torch.device('cuda')
    model.to(device)
    print(device)
    print(torch.cuda.device_count())
    time.sleep(3)
    
    
    for epoch in range(num_epochs):
        running_loss = 0.0
        for i, (images, labels) in enumerate(train_loader):
            images, labels = images.to(device), labels.to(device)
            optimizer.zero_grad()
            outputs = model(images)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()
            running_loss += loss.item()
        print('Epoch [%d/%d], Loss: %.4f' % (epoch+1, num_epochs, running_loss/len(train_loader)))

    # Evaluation du modèle sur les données de validation
    model.eval()
    with torch.no_grad():
        correct = 0
        total = 0
        for images, labels in val_loader:
            images, labels = images.to(device), labels.to(device)
            outputs = model(images)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
        print('Accuracy of the model on the validation set: %d %%' % (100 * correct / total))

    # Export du modèle en format ONNX
    model.eval()
    example_input = torch.rand(1, 3, 224, 224).to(device)
    output_path = 'torchNet_test5.onnx'
    torch.onnx.export(model, example_input, output_path)
    print('Model saved to %s' % output_path)


if __name__ == '__main__':
    main()