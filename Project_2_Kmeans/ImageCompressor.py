from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import Slider, Button
from numpy import random as rand

class ImageCompressor:
    def __init__(self, image_path):
        self.original_image = self.load_image(image_path)
        self.kmeans_images = {}
        self.last_k = 0  # Stores last used k-value for this image

    def load_image(self, image_path):
        with Image.open(image_path) as img:
            return np.array(img) / 255.0  # Normalize to range [0, 1]

    def kmeans(self, num_clusters):
        if num_clusters == 0:
            self.kmeans_images[num_clusters] = self.original_image
            return

        height, width, channels = self.original_image.shape
        pixel_data = self.original_image.reshape((height * width, 3))

        # Initialize centroids randomly from existing pixels
        initial_centroids = pixel_data[rand.choice(pixel_data.shape[0], num_clusters, replace=False)]

        stable = False
        max_iterations = 50
        iteration = 0

        while not stable and iteration < max_iterations:
            iteration += 1

            # Assign each pixel to the nearest centroid
            distances = np.linalg.norm(pixel_data[:, np.newaxis] - initial_centroids, axis=2)
            pixel_clusters = np.argmin(distances, axis=1)

            # Compute new centroids
            new_centroids = np.array([
                pixel_data[pixel_clusters == i].mean(axis=0) if np.any(pixel_clusters == i)
                else initial_centroids[i]
                for i in range(num_clusters)
            ])

            # Check for convergence
            stable = np.allclose(new_centroids, initial_centroids, atol=1e-5)
            initial_centroids = new_centroids

        # Assign each pixel its centroid color
        compressed_pixels = np.array([initial_centroids[cluster] for cluster in pixel_clusters])
        compressed_image = compressed_pixels.reshape((height, width, 3))

        # Store compressed image
        self.kmeans_images[num_clusters] = compressed_image
        self.last_k = num_clusters  # Update last k-value used

    def get_image(self, k):
        # Returns the original image if k=0, otherwise the k-means compressed image.
        if k not in self.kmeans_images:
            self.kmeans(k)
        return self.kmeans_images[k]

# List of image filenames
image_filenames = ["test1.jpg", "test2.jpg", "test3.jpg", "test4.jpg", "test5.jpg"]

# Create ImageCompressor objects for all images with a base width of 128 pixels
image_compressor_list = [ImageCompressor(filename) for filename in image_filenames]

# Initialize plot
fig, ax = plt.subplots(figsize=(8, 8))
plt.subplots_adjust(bottom=0.3)
ax.set_title("K-Means Adjustment")
ax.axis('off')

# Display the initial image
current_image_index = 0
img_display = ax.imshow(image_compressor_list[current_image_index].original_image)

# Add a slider for K-Means control
ax_slider = plt.axes([0.25, 0.1, 0.5, 0.03])
slider = Slider(ax_slider, 'K-Means', 0, 64, valinit=0, valstep=1)

# Update function for slider
def update(val):
    k_value = int(slider.val)
    compressor = image_compressor_list[current_image_index]

    adjusted_image = compressor.get_image(k_value)  # Get image using the new method
    img_display.set_data(adjusted_image)
    fig.canvas.draw_idle()

# Connect the slider to the update function
slider.on_changed(update)

# Function to change the displayed image
def change_image(direction):
    global current_image_index

    if direction == 'next':
        current_image_index = (current_image_index + 1) % len(image_compressor_list)
    elif direction == 'prev':
        current_image_index = (current_image_index - 1) % len(image_compressor_list)

    last_k = image_compressor_list[current_image_index].last_k
    slider.set_val(last_k)

# Add "Previous" and "Next" buttons
ax_prev = plt.axes([0.1, 0.01, 0.1, 0.075])
ax_next = plt.axes([0.8, 0.01, 0.1, 0.075])

button_prev = Button(ax_prev, 'Previous')
button_next = Button(ax_next, 'Next')

# Define the button callbacks
button_prev.on_clicked(lambda event: change_image('prev'))
button_next.on_clicked(lambda event: change_image('next'))

# Show the plot
plt.show()
