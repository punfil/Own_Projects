#! /usr/bin/env python3
import opendatasets as od
import os
import random
import shutil

DATASET_URL = 'https://www.kaggle.com/alxmamaev/flowers-recognition/flowers'
od.download(DATASET_URL)

root_dir = "/home/wpanfil/Documents/Own_Projects/jezyki_skryptowe/proj5/flowers-recognition/flowers"
train_dir = os.path.join(root_dir, "train")
val_dir = os.path.join(root_dir, "validate")
test_dir = os.path.join(root_dir, "test")

train_split = 0.7
val_split = 0.15
test_split = 0.15

flowers = list(os.listdir(root_dir))

os.makedirs(train_dir, exist_ok=False)
os.makedirs(val_dir, exist_ok=False)
os.makedirs(test_dir, exist_ok=False)

for flower_category in flowers:
    flower_category_path = os.path.join(root_dir, flower_category)
    os.makedirs(os.path.join(train_dir, flower_category), exist_ok=False)
    os.makedirs(os.path.join(val_dir, flower_category), exist_ok=False)
    os.makedirs(os.path.join(test_dir, flower_category), exist_ok=False)
    images = os.listdir(flower_category_path)
    random.shuffle(images)
    num_train = int(len(images) * train_split)
    num_val = int(len(images) * val_split)
    num_test = len(images) - num_train - num_val
    train_images = images[:num_train]
    val_images = images[num_train:num_train + num_val]
    test_images = images[num_train + num_val:]
    for image in train_images:
        src = os.path.join(flower_category_path, image)
        dst = os.path.join(train_dir, flower_category, image)
        shutil.copy(src, dst)

    for image in val_images:
        src = os.path.join(flower_category_path, image)
        dst = os.path.join(val_dir, flower_category, image)
        shutil.copy(src, dst)

    for image in test_images:
        src = os.path.join(flower_category_path, image)
        dst = os.path.join(test_dir, flower_category, image)
        shutil.copy(src, dst)

exit(0)
