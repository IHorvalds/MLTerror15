import argparse
import pathlib
import sys
sys.path.insert(1, 'D:\\Practice Apps\\AudioPluginsDev\\NeuAmpNetDemo\\')
from RTNeural.python.model_utils import save_model
import tensorflow as tf
import argparse


def main():
    parser = argparse.ArgumentParser(description='Convert model .pb file to .json')
    parser.add_argument("--model",  type=str, required=True)
    args = parser.parse_args()

    model = tf.keras.models.load_model(args.model, compile=False)
    print(model.summary())
    save_model(model, pathlib.Path(args.model).parts[-1] + ".json")

if __name__ == "__main__":
    main()