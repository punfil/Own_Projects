#! /usr/bin/env python3
import os
import shutil
from PIL import Image
import torchvision.transforms as T
from torch.utils.data import Dataset


ROOT_DIR = './flowers-recognition'
DATA_DIR = './flowers-recognition/flowers'


def prepare_files():
    dup_dir = DATA_DIR + '/flowers'
    if os.path.exists(dup_dir) and os.path.isdir(dup_dir):
        shutil.rmtree(dup_dir)
    rename_files()


def rename_files():
    classes = os.listdir(ROOT_DIR)
    for par_class in classes:
        for file in os.listdir(ROOT_DIR + '/' + par_class):
            if file.endswith('jpg'):
                os.rename((ROOT_DIR + '/' + par_class + '/' + file),
                          (ROOT_DIR + '/' + par_class + '/' + par_class + "_" + file))


def parse_species(fname):
    parts = fname.split('_')
    return parts[0]


def open_image(path):
    with open(path, 'rb') as f:
        img = Image.open(f)
        return img.convert('RGB')



class FlowersDataset(Dataset):
    def __init__(self, root_dir, transform):
        super().__init__()
        self.root_dir = root_dir
        self.files = []
        self.classes = [fname for fname in os.listdir(root_dir) if fname != 'flowers']
        for classes in self.classes:
            for file in os.listdir(root_dir + '/' + classes):
                if file.endswith('jpg'):
                    self.files.append(file)
        self.transform = transform

    def __len__(self):
        return len(self.files)

    def __getitem__(self, i):
        fname = self.files[i]
        species = parse_species(fname)
        fpath = os.path.join(self.root_dir, species, fname)
        img = self.transform(open_image(fpath))
        class_idx = self.classes.index(species)
        return img, class_idx


def main():
    prepare_files()
    img_size = 64
    stats = ((0.485, 0.456, 0.406), (0.229, 0.224, 0.225))
    transform = T.Compose([T.Resize((img_size, img_size)),
                       T.RandomCrop(64, padding=4, padding_mode='reflect'),
                       T.RandomHorizontalFlip(),
                       T.ColorJitter(brightness=0.1, contrast=0.1, saturation=0.1, hue=0.1),
                       T.ToTensor(),
                       T.Normalize(*stats,inplace=True)])
    dataset = FlowersDataset(DATA_DIR, transform=transform)
    print(len(dataset))


if __name__ == "__main__":
    main()
