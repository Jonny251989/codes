import grpc
from concurrent import futures
import torch
from transformers import AutoTokenizer, AutoModelForSequenceClassification
import toxicity_classifier_pb2
import toxicity_classifier_pb2_grpc

class ToxicityClassifierServicer(toxicity_classifier_pb2_grpc.ToxicityClassifierServicer):
    def __init__(self):
        self.model_name = 's-nlp/russian_toxicity_classifier'
        print("Loading tokenizer and model...")
        self.tokenizer = AutoTokenizer.from_pretrained(self.model_name)
        self.model = AutoModelForSequenceClassification.from_pretrained(self.model_name)
        print("Model and tokenizer loaded successfully.")

    def ClassifyMessage(self, request, context):
        print("Received message:", request.message)
        try:
            print("Tokenizing the message...")
            inputs = self.tokenizer(request.message, return_tensors="pt", truncation=True, padding=True, max_length=512)
            print("Processing the model...")
            with torch.no_grad():
                logits = self.model(**inputs).logits
            probs = torch.sigmoid(logits).squeeze().numpy()
            print("Toxicity probability:", probs[1])
            return toxicity_classifier_pb2.MessageResponse(toxicity_probability=probs[1])
        except Exception as e:
            print("Error processing message:", e)
            raise

def server():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    toxicity_classifier_pb2_grpc.add_ToxicityClassifierServicer_to_server(ToxicityClassifierServicer(), server)
    server.add_insecure_port('[::]:50051')
    print("Server started at [::]:50051")
    server.start()
    server.wait_for_termination()

if __name__ == '__main__':
    server()
