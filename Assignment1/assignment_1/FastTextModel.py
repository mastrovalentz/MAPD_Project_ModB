from dask.distributed import Client, LocalCluster
from dask import delayed

import dask.bag as db
import dask.dataframe as ddf
import os
import json
from operator import itemgetter
from operator import add

from nltk.tokenize import RegexpTokenizer
from nltk.corpus import stopwords
from nltk import word_tokenize

from collections import Counter

import time

import numpy as np

import io
import fasttext


#cluster=LocalCluster(n_workers=4)
#client= Client(cluster)

def embedd(text, model):
    text_split = text.split()
    text_embedded = []
    #just to be safe but the .bin model does predict everything
    for t in text_split:
        try:
            text_embedded.append(model[t])
        except:
            pass
    text_string = [["%.8f" % n for n in w] for w in text_embedded]

    return text_string

def embedd_dict(dict, model):
    return{
        "paper_id": dict["paper_id"],
        "title": embedd(dict['title'], model)
    }


def flatten(reco):
    return {
        "paper_id": reco[0],
        "title": reco[1]['title']
    }



def make_titles():
    filename = os.path.join('data', 'papers_in_json_singleline', '*.json')
    lines = db.read_text(filename)
    js = lines.map(json.loads).repartition(10)
    metas = js.pluck(["paper_id", "metadata"])
    ft = fasttext.load_model('/home/alessandro/Downloads/cc.en.300.bin')

    #ft.get_word_vector("hello")
    titles = metas.map(flatten).compute()
    ed = [embedd_dict(d, ft) for d in titles]
    print(ed[1])
    
    for i in range(len(ed)):
        fname="data/embedded_papers/embTitle"+str(i)+".json"
        with open (fname, 'w') as wf:
            json.dump(ed[i], wf)
            
    #non funziona, crea i file ma non salva i dati
    #embedded_titles = db.from_sequence(ed, npartitions=20)
    #embedded_titles.to_textfiles   ("data/embedded_papers/*.json.gz")
    #print(embedded_titles.take(1))

    

def make_model():
    
    ft = fasttext.load_model('/home/alessandro/Downloads/cc.en.300.bin')
    return ft


def try_func(model, text):
    print(model.get_word_vector(text))


if __name__ == "__main__":
    cluster=LocalCluster(n_workers=4)
    client= Client(cluster)   
    
    make_titles()
    client.close()
    cluster.close()