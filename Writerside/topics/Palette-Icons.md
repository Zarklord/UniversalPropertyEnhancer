# Palette Icons
<link-summary>Use UPE to add custom palette icons to your part pages</link-summary>
<web-summary>Use UPE to add custom palette icons to your part pages</web-summary>
<card-summary>Use UPE to add custom palette icons to your part pages</card-summary>

All palette icon data is defined inside the <path>paletteicons</path> folder, so you must first create that folder in your mod.

Create a <path>.prop</path> file with a unique name in the <path>paletteicons</path> folder.
<warning>
    <p>
        Ensure that all prop files in the <path>paletteicons</path> folder have a unique name, if they share a name with another mod, it won't work.
    </p>
</warning>
<tip>
    <p>
        It's usually a good idea to use your mod's name, to reduce the chance of conflicting with other mods.
    </p>
</tip>

Add the following properties to that prop file:

- ```
  key paletteSetButtonBackgroundIcon common~!bgicon.png
  ```
  Edit `common~!bgicon.png` to point to your icon background.
- ```
  key paletteSetButtonIcon common~!icon.png
  ```
  Edit <code>common~!icon.png</code> to point to your icon.
- ```
  key paletteSetButtonLayout layouts~!0x98F661C7.layout
  ```
  Usually, you don't need to edit this.
- ```
  texts paletteSetName
      (0x0!0x0) ""
  end
  ```
  This is the text that appears when you hover over your icon, point it to your locale property and your text.
- ```
  int32 paletteSetSequenceNumber 1025
  ```
  This determines the ordering of palette icons in relation to each other
  <warning>
      <p>
          This does not control the ordering of pages, so if this is set improperly,
          its possible for your icon to appear out of order compared to the pages it's on.
      </p>
  </warning>

Then for every page you want to have your icon, add the following property:
```
int32 palettePageSetId hash(PALETTEICONPROP)
```
Editing `PALETTEICONPROP` to be the name of your palette icon prop file (excluding the file extension).