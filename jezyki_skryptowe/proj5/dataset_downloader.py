#! /usr/bin/env python3
import opendatasets as od

DATASET_URL = 'https://www.kaggle.com/alxmamaev/flowers-recognition/flowers'
od.download(DATASET_URL)