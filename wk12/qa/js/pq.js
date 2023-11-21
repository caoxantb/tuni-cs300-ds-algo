/**
 * 
 * 
// Example usage:
//   const priorityQueue = new PriorityQueue();
  
//   priorityQueue.add({ distance: 32, node: [0, 0] });
//   priorityQueue.add({ distance: 45, node: [1, 1] });
//   priorityQueue.add({ distance: 12, node: [2, 5] });
  
//   console.log(priorityQueue.remove());
//   console.log(priorityQueue.remove());
//   console.log(priorityQueue.remove());
  
 * 
 */
class PriorityQueue {
    constructor(comparator = (a, b) => a.distance - b.distance) {
      this.queue = [];
      this.comparator = comparator;
    }
  
    add(element) {
      this.queue.push(element);
      this.queue.sort(this.comparator);
    }
  
    remove() {
      if (this.isEmpty()) {
        throw new Error('PriorityQueue is empty');
      }
      return this.queue.shift();
    }
  
    isEmpty() {
      return this.queue.length === 0;
    }
  }
  
