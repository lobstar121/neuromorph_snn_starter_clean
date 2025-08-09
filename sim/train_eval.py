import numpy as np
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score, confusion_matrix

def spike_count_features(spikes_batch):
    """
    spikes_batch: (B, T, N) -> (B, N)
    """
    return spikes_batch.sum(axis=1).astype(np.float32)

def train_readout(X_counts, y):
    # 클래스 쏠림/스케일 이슈를 완화하기 위해 균형 가중치 + 여유 있는 max_iter
    clf = LogisticRegression(max_iter=2000, class_weight="balanced")
    clf.fit(X_counts, y)
    return clf

def eval_readout(clf, X_counts, y):
    y_pred = clf.predict(X_counts)
    acc = accuracy_score(y, y_pred)
    cm = confusion_matrix(y, y_pred)
    return acc, cm
