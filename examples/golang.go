in

import (
	"fmt"
	"sync"
	"time"
)

// Document represents our shared resource that needs to be "ready".
type Document struct {
	ID      string
	Content string
	Ready   bool // This flag is primarily for demonstration; the channel handles the actual signal.
}

// prepareDocument simulates the process of making a document ready.
// It takes a channel and closes it when the document is ready, signaling all waiting goroutines.
func prepareDocument(doc *Document, docReadySignal chan struct{}) {
	fmt.Printf("[Preparer] Starting to prepare document '%s'...\n", doc.ID)
	time.Sleep(3 * time.Second) // Simulate intensive preparation work

	doc.Content = "This is the super important document content!"
	doc.Ready = true // Mark as ready

	fmt.Printf("[Preparer] Document '%s' is now ready. Signaling all workers!\n", doc.ID)
	close(docReadySignal) // Closing the channel unblocks all goroutines waiting on it.
}

// worker simulates a goroutine that needs to process the document.
// It waits on the docReadySignal channel before proceeding.
func worker(workerID int, doc *Document, docReadySignal chan struct{}, wg *sync.WaitGroup) {
	defer wg.Done() // Ensure WaitGroup counter is decremented when the worker finishes.

	fmt.Printf("[Worker %d] Started. Waiting for document '%s' to be ready...\n", workerID, doc.ID)

	// This is where the worker blocks until the docReadySignal channel is closed.
	// Receiving from a closed channel immediately yields the zero value, unblocking the goroutine.
	<-docReadySignal

	fmt.Printf("[Worker %d] Document '%s' is ready! Processing content: '%s'\n", workerID, doc.ID, doc.Content)
	// Simulate some work after document is ready
	time.Sleep(500 * time.Millisecond)
	fmt.Printf("[Worker %d] Finished processing document '%s'.\n", workerID, doc.ID)
}

func main() {
	fmt.Println("Starting document processing simulation...")

	// 1. Create our "document"
	myDocument := &Document{
		ID:    "Report-2025-Q3",
		Ready: false,
	}

	// 2. Create a channel to signal when the document is ready.
	// A struct{} is used because we only care about the signal, not any actual data.
	docReadySignal := make(chan struct{})

	// 3. Use a WaitGroup to wait for all worker goroutines to complete their tasks
	var wg sync.WaitGroup
	const numWorkers = 5

	// 4. Start the document preparer goroutine
	go prepareDocument(myDocument, docReadySignal)

	// 5. Start multiple worker goroutines
	for i := 1; i <= numWorkers; i++ {
		wg.Add(1) // Increment the WaitGroup counter for each worker
		go worker(i, myDocument, docReadySignal, &wg)
	}

	// 6. Wait for all worker goroutines to finish their processing
	wg.Wait()
	fmt.Println("All workers have finished processing. Simulation complete.")
}
∂
